# `Gun::calcCharacterCollision` wave 118 (w6615): the residual is a *coupled*
# MBP-rotation + greedy-RA decision; ~700 further variants, no source shape
# reaches the original's (this=r9, enemy=r8, td=r6, i*3=r4) + rotated k-loop

Worker: w6615. Branch: `wave118-w6615`, base master `3d4a98a3e`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
650 instructions. Scratch: `/dev/shm/w6615/`.

## 0. Result

| build | gate byte/linked | target pct | matched | size |
|---|---:|---:|---:|---:|
| before (HEAD `3d4a98a3e`) | **4501 / 4501** | **93.3846** (report 93.4) | 607/650 | 1984 |
| after this pass | **4501 / 4501** | **93.3846** | 607/650 | 1984 |

No code change: analysis only. `check` after: byte 4501/4524, linked 4501/4524,
size-exact 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
`RATCHET PASS`. Structural state unchanged: 27 ELF sections, file size
2,187,328 B, `.text` 0x1337c8, `.ARM.exidx` 0x6f70, `.ARM.extab` 0x2628c
(original 0x26298 — the documented 12 B shortfall, `docs/VALIDATION.md:101`),
`.rodata` 0x41c27, `.data` 0x296c, `.bss` 0x69ac.

## 1. What the original actually is

The shipped function has **all four** of these properties at once:

| property | shipped | HEAD | plain-`if` body |
|---|---|---|---|
| `this` | r9 | r9 | **r4** |
| `enemy` | r8 | r8 | **r6** |
| `td` (`&this->targetDir`) | r6 | r6 | **r9** |
| `i*3` | **r4** | **r5** | (not kept; `fp`=radius) |
| `i` | fp | fp | r5 |
| k-loop block layout | **rotated** `[body,then,latch,header]` | **not rotated** `[header,body,latch,then…]` | rotated |
| size | 1984 | 1984 | 1908 |

So the shipped build is *not* simply "HEAD with a different register" nor
"plain-`if` with a different register": it combines HEAD's register map with
the plain-`if` loop rotation, and additionally moves `i*3` from r5 to r4.

## 2. MBP: why the k-loop is rotated in the original but not in HEAD

`-mllvm -print-after-all -mllvm -filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv`
on the NDK clang gives the real MIR. Pre-MBP block order is the ISel order

```
bb.89(header) bb.90(body) bb.91(then) bb.92 bb.93 bb.94 bb.95(latch)
```

and MBP emits

```
bb.89 bb.90 bb.95 bb.91 bb.92 bb.93 bb.94
```

i.e. chain `[89, 90, 95]` + appended `[91..94]`. `rotateLoop` bails because
`Bottom = bb.94` whose only successor is `bb.95`, not `Top = bb.89`
(`!Bottom->isSuccessor(Top)`), while `ExitingBB(bb.89)->isSuccessor(bb.90)` is
true. The shipped layout `[body, then, latch, header]` is the rotation of
`[89, 90, 91, 93, 94, 95]`, which requires `selectBestSuccessor(bb.90) = bb.91`,
i.e. a **hot** `distance < magnitude` edge.

MIR edge probabilities in HEAD:

```
bb.90 successors: bb.91(0.05%), bb.95(99.95%)   <- __builtin_expect(...,0)
bb.89 successors: bb.90(96.88%), bb.31(3.12%)
bb.95 successors: bb.89(200%)
```

With a plain `if` the edge is 50/50, `bb.91` is the first successor and wins the
tie, the chain becomes `[89,90,91,93,94,95]`, and `rotateLoop` produces exactly
the shipped layout. But the plain `if` also removes the `%306` spill whose
local child (`%455`) evicts `this` from r4 to r9, so the whole register map
changes (`this=r4`, `enemy=r6`, `td=r9`, `fp`=radius, 1908 B, 64.5 %).

## 3. RA: why `i*3` lands in r5

`i*3` (`%98`, live 0x15306c–0x153436) is a *global* range; the `&delta`
(`sp+104`) value that steals r4 is a rematerialisable stack address. In the
greedy-RA order `this` (`%56`, hinted, size 16616) takes r4 first, `td` (`%11`,
12174) takes r5, and `%98` can only get r9; it is re-queued to r5 only after the
late `%455` local child evicts `this` to r9. To get `i*3 = r4` the `this`
eviction must happen before `%98` is allocated, i.e. a **local-range** spill
child must be created earlier — which is exactly what the cold branch does, but
the cold branch is also what kills the rotation. The two decisions are coupled
through the same spill.

## 4. New measurements this wave (official verifier path)

Harness: `q.sh` (touch + `link_so` + single-function score) and a raw-objdump
register probe; every number below is `pct / size` with `this/i3/enemy` read
from the linked `.so`.

### 4.1 Hot (plain) `distance < magnitude` body × single `__builtin_expect`
on each of 36 branch sites × {0,1} (72 builds)

| site | value | pct | size | this | i3 | enemy |
|---|---|---:|---:|---|---|---|
| `if (mine) radius *= 5` | 0 | **82.6** | 1980 | r9 | **r4** | r6 |
| `else if (scatter)` (radius) | 1 | 80.5 | 1980 | r9 | **r4** | r6 |
| everything else | 0/1 | ≤ 64.5 | 1908–1968 | r4/r1 | r0/– | r6 |

Nothing reaches `enemy = r8`. `mine`(0)+`empDamage>0`(0) = 83.9/1984 (still
`enemy=r6`).

### 4.2 `mine`-radius-cold body (`plain` + `expect(mine,0)`) × single expect
on each of 36 sites (68 builds)

Best 83.9 (`empDamage>0`(0), 1984, this=r9 i3=r4 enemy=r6); `enemy` stays r6 for
**every** combination. `td` is r8 in this family (swapped with `enemy`).

### 4.3 HEAD (cold `distance`) body × single expect (68 builds) and
`egoFlag`(0) + X (68 builds)

The only shapes that keep `this=r9, enemy=r8` *and* reach `i3=r4`:

| variant | pct | size | this | i3 | enemy | rotated? |
|---|---|---:|---|---|---|---|
| `egoFlag`(0) + `if (mine) continue`(1) | **82.8** | 1980 | r9 | **r4** | **r8** | no |
| `egoFlag`(0) + `isAsteroid`(1) | 63.8 | 2016 | r9 | r4 | r8 | no |
| `if (mine) radius*=5`(1) | 73.4 | 1996 | r9 | r4 | r8 | no |
| `else if (scatter)`(0) | 72.6 | 1992 | r9 | r4 | r8 | no |

`egoFlag`(0) alone is 59.4/1956 (`this=r1`); the second expect is what restores
`this=r9`. All four are *not* rotated, so they cannot converge on the shipped
layout.

### 4.4 Source-shape variants (all measured, none > 93.4)

`int`/`bool` for `mine`, `scatter`, `explosive`, `explodeOnContact`;
`weaponType` non-const; `Vector position/velocity/enemyPosition` hoisted to
function scope (65.5); `Array<Vector>&` aliases (93.4, CSE'd); `td` as
reference / `this->targetDir` direct / declared inside the scatter block /
inside the outer loop (all 93.4); radius via `if`/`if-else`/ternary/separate
`if`s/`switch`; `i != count`, `k != size`, `e != size`, `i += 1`, `k += 1`,
`i <= count-1` (92.8–93.4); `float r` inlined (81.4); `outer` inlined (56.8) or
split (93.4); `ki` const (93.4); `enemy` declared outside the outer loop
(93.4); scatter `playerPosition`/`delta` in a lambda or a nested scope (93.4);
`hitFlags`/`hitPositions` after the k-loop (59.8); `Vector ep` in the k-loop
(93.4); `Player *target` new local in the k-loop (56.8); `data()[k]` (93.4);
`count` (93.4); `int k` (93.4); `while` (70.5); `n = size()` cached (67.9);
`k = 0; do {…}` (build error / 1956 B family); `explosive`/`explodeOnContact`
declaration order (93.4); `this->enemies == nullptr` moved after the weaponType
block (85.3).

Random search: 150 + 90 + 120 samples over random 1–3-expect subsets plus
statement mutations (`position`/`velocity` swap, `data()[i]`→`[i]`,
`unsigned`→`int`), no score above 93.4.

### 4.5 k-loop rewrites on the HEAD body

`Player *target` local (56.8/1884), `data()[k]` (93.4), `count` (93.4),
`int k` (93.4), `while` (70.5/1940), `n` cached (67.9/1960), `do{}while`
(1956 B family), `hitFlags` after the loop (59.8/1956), `Vector ep` (93.4),
`*td = enemy->getPosition() - position` (1956 B family), `*td = ep - position`
(1956 B family).

## 5. Conclusion

The shipped function simultaneously requires:

1. `this = r9` (greedy-RA eviction of `this` from r4 by a local spill child),
2. `enemy = r8`, `td = r6` (HEAD's map),
3. `i*3 = r4` (the `&delta` stack address must lose r4),
4. the k-loop rotated (the `distance < magnitude` edge must be **hot**).

(1) and (3) need a cold branch to create the early spill; (4) needs the
`distance` edge itself to be hot. Every source shape that satisfies (4) by
making the distance edge hot loses (1)/(2) (plain: `this=r4`, `enemy=r6`,
`td=r9`, 64.5 %); every shape that keeps (1)/(2) keeps the edge cold and loses
(4). The only shapes that satisfy (1)+(2)+(3) (`egoFlag(0)+mine_continue(1)`
82.8, `mine_radius(1)` 73.4, `scatter_radius(0)` 72.6) all leave the k-loop
unrotated and are 4–32 B off in size.

Best measured variant remains the HEAD body at **93.3846 % / 1984 B**.
≈ 700 variants were measured this wave on top of the ≈ 400 from waves
106–117; none exceeded 93.3846.

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

MIR block layout / branch weights:

```
<clang++> -mllvm -print-after-all \
  -mllvm -filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv -c src/game/weapons/Gun.cpp
# "IR Dump After Branch Probability Basic Block Placement" -> block order
# "IR Dump After Greedy Register Allocator" -> physical-register MIR
```

Register probe: `arm-linux-androideabi-objdump -d cmake-build-match/libgof2hdaa.so`
then read `mov r9, r0` (`this`), `add.w rX, fp, fp, lsl #1` (`i*3`),
`ldr.w rY, [r0, r2, lsl #2]` (`enemy`), `str rZ, [sp, #24]` (`td`).
