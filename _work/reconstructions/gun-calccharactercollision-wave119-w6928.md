# `Gun::calcCharacterCollision` wave 119 (w6928): a shape with loop rotation
# **and** `this=r9` **and** `i*3=r4` exists (`plain distance` + hot `scatter`),
# but it costs `enemy`/`td`; the four shipped properties still do not co-occur

Worker: w6928. Branch: `wave119-w6928`, base master `dfe78cca9`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
650 instructions. Scratch: `/dev/shm/w6928/` (not tracked).

## 0. Result

| build | gate byte/linked | target pct | size |
|---|---:|---:|---:|
| before (HEAD `dfe78cca9`) | **4505 / 4505** | **93.4000** | 1984 |
| after this pass | **4505 / 4505** | **93.4000** | 1984 |

No code change: analysis only. `check` after: `RATCHET PASS`, byte 4505/4524,
linked 4505/4524, size-exact 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0. Structural state unchanged: 27 ELF sections, file size 2,187,328 B.
No `.text`/`.ARM.extab`/section change.

Read together with `gun-calccharactercollision-wave119-w6904.md` (LICM / stack
colouring root cause) and `...-wave119-w6713.md` (frame-index rules); this note
only records what is new.

## 1. The shipped map vs HEAD (re-measured)

| property | shipped | HEAD |
|---|---|---|
| `this` | r9 | r9 |
| `enemy` | r8 | r8 |
| `td` (`&this->targetDir`) | r6 | r6 |
| `i` | fp (spilled to sp+40) | fp (spilled to sp+40) |
| `i*3` | **r4** | **r5** |
| `playerPosition` (`pp`) | **r4 / sp+44** | r4 / sp+104 |
| `delta` (`enemyPosition - pp`) | **r5 / sp+104** | r5 / sp+44 |
| k-loop layout | **rotated** `[body,then,latch,header]` | `[header,body,latch]` + `then` |
| k-loop `&enemy->getPosition()` sret | recomputed in body `0x153444` | hoisted to i-loop body `0x153260` |

The `cmake-build-match` `.so` left behind by the previous worker was stale (it
held the abandoned `expect(scatter,0)` experiment: 1992 B / 655 insns). The
baseline here was rebuilt from a clean `git checkout` of `Gun.cpp`.

## 2. New measurements (proxy = tools/verify normaliser + SequenceMatcher,
## masked call targets; official HEAD proxy is 93.85 = official 93.40)

### 2.1 The statement form and the one-line expression form are **not** identical

The two-line expression form is the one that transposes the slots; the
one-line form collapses back to the statement form:

| variant | proxy | size | first two `add rX,sp,#44|104` |
|---|---:|---:|---|
| `Vector pp = getPosition(); VectorLength(ep - pp)` (HEAD) | **93.85** | 1984 | `r4/104` (pp), `r5/44` (delta) |
| `VectorLength(ep - getPosition())` on **one** line | 93.85 | 1984 | `r4/104`, `r5/44` |
| `VectorLength(\n ep - getPosition())` on **two** lines | 93.08 | 1984 | `r5/44` (pp), `r4/104` (delta) |
| `Vector delta = ep - getPosition(); VectorLength(delta)` | 93.08 | 1984 | `r5/44`, `r4/104` |
| `VectorLength(AEMath::operator-(ep, getPosition()))` | 93.08 | 1984 | `r5/44`, `r4/104` |
| `const Vector &pp = getPosition();` | 93.85 | 1984 | identical to HEAD |
| `Vector pp(getPosition());` (copy-ctor) | 93.85 | 1984 | identical to HEAD |

So the "expression form" datapoint of w6713 reproduces **only** when the call
is split across two source lines (the line split changes the temporary
materialisation order, hence the frame indices).

### 2.2 `delta`-first declarations (frame-index experiments)

| variant | proxy | size | slots |
|---|---:|---:|---|
| `Vector delta; Vector pp = getPosition(); delta = ep - pp;` | 65.64 | 1964 | `r4/44`, `r4/104` |
| `Vector delta = ep; Vector pp = getPosition(); delta -= pp;` | 80.34 | 1992 | `r5/44`, `r4/104` |
| `Vector delta(ep); delta -= getPosition();` | 80.34 | 1992 | `r5/44`, `r4/104` |

`delta`-first does move `delta` into fi#4 (→ sp+104, matching the shipped
slot), but it also moves *both* address temporaries into r4 or transposes the
registers; nothing keeps `pp=r4`.

### 2.3 i-loop body reorderings (all keep `i3=r5`, none beat HEAD)

| variant | proxy | size |
|---|---:|---:|
| swap `position`/`velocity` | 90.31 | 1984 |
| `enemyPosition` first | 87.54 | 1984 |
| `velocity, enemyPosition, position` | 88.31 | 1984 |
| `radius` first | 76.54 | 1968 |
| `enemyPosition, radius, position, velocity` | 69.66 | 1960 |
| `*(data()+i)` / `operator[]` / `int i` / `unsigned int i` | 93.85 | 1984 |
| `const Vector position/velocity` | 93.85 | 1984 |
| `for (...; i != count; ...)` / `i++` | 93.69 / 93.85 | 1984 |
| `const unsigned i3 = i*3; data()[i3/3]` | 60.02 | 1964 |
| extra `Vector spare;` at i-loop top (DCE'd) | 93.85 | 1984 |
| extra `Vector spare = upVectors.data()[i]; (void)spare;` | 93.85 | 1984 |

### 2.4 k-loop variants

| variant | proxy | size | rot |
|---|---:|---:|---|
| cached `unsigned n = enemies->size()` | 69.61 | 1960 | — |
| `while (k < size)` | 72.39 | 1940 | — |
| `enemies->data()[k]` | 93.85 | 1984 | False |
| `int k` / `k != size` / `k` declared outside | 93.85 | 1984 | False |
| `if (magnitude > distance)` / `!(distance >= magnitude)` / `distance-magnitude < 0` | 93.85 | 1984 | False |
| plain `if` (no expect) | 67.03 | 1908 | **True** |

### 2.5 Matrix / view forms (all codegen-identical to HEAD, 93.85 / 1984)

`Matrix *vp = ...; Matrix view = *vp;`, `const Matrix *vp`, copy-ctor
`Matrix view((*...))`, `Matrix tmp = view; MatrixSetTranslation(tmp,...)`,
trailing `(void)view;`.

Two that *do* change things:

| variant | proxy | size | map |
|---|---:|---:|---|
| `Matrix view;` at i-loop top, `view = *CameraGetLocal(...)` in the geometries branch | 55.67 | **2024** | `i3=r4`, `td=r1` |
| move `Matrix view = ...` above the two `SetAnimationState` calls | 87.16 | 1988 | `i3=r5` |

The first is the only *non-expect* shape found that moves `i*3` to r4: keeping
the `view` slot live across the whole i-loop body stops it merging with the
scatter temporaries (+40 B frame, and `td` falls out of r6).

### 2.6 `__builtin_expect` on the mine/radius and scatter guards

| variant | proxy | size | map |
|---|---:|---:|---|
| `expect(mine,0)` on `radius *= 5` | 90.07 | 1984 | `i3=r6` |
| `expect(mine,1)` on `radius *= 5` | 75.34 | 1996 | `i3=r4` |
| `expect(scatter,0)` | 74.79 | 1992 | `i3=r4` |
| `expect(scatter,1)` | 87.21 | 1980 | `i3=r6` |
| two-line anon pp + `expect(mine,1)` | 75.34 | 1996 | `this=r9, enemy=r8, td=r6, i3=r4` (all four registers, correct slots!) |
| two-line anon pp + `expect(mine,0)` | 91.15 | 1984 | `i3=r6` |

`anon pp + expect(mine,1)` is the first shape seen with **all four shipped
register/slot properties at once**, but it costs 12 B of frame (1996) and
scores 75.34, so it is not usable.

### 2.7 NEW: `plain distance` + hot `scatter` — rotation **and** `this=r9`
### **and** `i*3=r4` together

Replacing the cold `expect(distance < magnitude, 0)` with a plain `if` and
making the scatter guard hot (`expect(scatter, 1)`) gives, for the first time,
the *rotated* k-loop together with the shipped `this`/`i3`:

| variant | proxy | size | this | enemy | td | i3 | rot |
|---|---:|---:|---|---|---|---|---|
| HEAD | 93.85 | 1984 | r9 | r8 | r6 | r5 | False |
| plain `distance` | 67.03 | 1908 | r4 | r6 | r9 | r0 | True |
| **plain `distance` + `expect(scatter,1)`** | **82.83** | 1980 | **r9** | r6 | r2 | **r4** | **True** |
| + `expect(empDamage != kNoAttribute, 0)` | 83.69 | 1984 | r9 | r6 | r2 | r4 | True |
| cold `distance` + `expect(scatter,1)` | 87.21 | 1980 | r9 | **r8** | **r6** | r6 | False |

So rotation + `this=r9` + `i3=r4` is *reachable*; the cost is `enemy` (r6
instead of r8) and `td` (r2 instead of r6).  A 14-site `__builtin_expect`
sweep on top of `plain distance + expect(scatter,1)` (isActive, hitpoints,
mine flag, egoFlag, mine radius, empDamage, owner, enemies==nullptr,
bomb||mine, mine filter, getKIPlayer deadFlag/field_0x71, distance>20000 —
both polarities) never moved `enemy`/`td` back to r8/r6; the best was
**83.69 / 1984** (`+ expect(empDamage != kNoAttribute, 0)`).

`&&`-style conditions (`distance < magnitude && magnitude > 0`, `&& distance
>= 0`, `|| distance < -1`) all collapse to the plain-`if` map
(`this=r4, i3=r0, rot=True`, 66.93–67.03).

## 3. Why the cold case does not rotate (MBP trace)

From the `Branch Probability Basic Block Placement` MIR dump of HEAD:

```
bb.89 (k-loop header): successors bb.90 (96.88%), bb.31 (3.12%)   <- exit
bb.90 (body):          successors bb.91 (0.05%), bb.95 (99.95%)   <- cold `then`
bb.95 (latch):         successor  bb.89
final order: bb.89, bb.90, bb.95, bb.91, ...
```

`selectBestSuccessor(bb.90)` picks the latch, so the chain is
`[bb.89, bb.90, bb.95]` and `then` (bb.91) is placed after it. `rotateLoop`
would rotate this chain to `[bb.90, bb.95, bb.89]`, but it does not fire:
`findBestLoopExit` bails out at its first guard
(`if (!LoopBlockSet.count(*HeaderChain.begin())) return nullptr;`) because the
k-loop header bb.89 has already been merged into a chain with its preheader
bb.88 (which is outside the loop). With the plain `if`, the chain fed to
`findBestLoopExit` still starts at the header, so the exit block is found and
the rotation happens. That is the mechanical reason the rotation is coupled to
the cold branch weight, and it is not reachable by any spelling of the
comparison alone.

## 4. Conclusion

The four shipped properties remain mutually exclusive through the two knobs
that exist at source level:

* the **branch weight** of `distance < magnitude` selects between the shipped
  RA map (cold, no rotation) and the rotated layout (plain/hot, different RA);
* the **frame index** of the `playerPosition` / `delta` allocas selects between
  the shipped slots (delta first) and the shipped registers (`pp` first).

New this wave: `plain distance + expect(scatter,1)` proves that
`this=r9` + `i*3=r4` + rotation *can* co-occur, so the coupling is between
`(i*3, rotation)` and `(enemy, td)`, not an absolute conflict.  The best
measured variant is still the unchanged HEAD body at **93.4000 % / 1984 B**
(607/650 instructions).

## 5. Reproduction

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

Fast loop used for the ~120 variants of this wave: compile `Gun.cpp` alone
with the exact NDK r18b flags (`/dev/shm/w6928/cc.sh`), then normalise with
`tools/verify`'s own disassembler and `difflib.SequenceMatcher` (proxy is
monotone with the official metric; HEAD proxy 93.85 == official 93.40).
