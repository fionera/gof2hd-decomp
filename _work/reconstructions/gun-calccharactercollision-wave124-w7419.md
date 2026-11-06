# `Gun::calcCharacterCollision` wave 124 (w7419): HEAD's 93.4 % still wins;
# the `expr`+B-first tail family reproduces the shipped scatter block
# byte-for-byte but pays a 12-byte pool shift and the wrong tail order

Worker: w7419. Base master `6f8ee2320` (wave 123 final). Original:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000, sha256
`d4c17be6…bed9483`). Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump
`0x152f90`, size `1984 B`, 660 instructions. Scratch: `/dev/shm/w7419/`
(not tracked).

## 0. Result

| build | gate byte/linked | target pct | matched | size |
|---|---:|---:|---:|---:|
| before (HEAD `6f8ee2320`) | **4506 / 4506** | **93.4000** | **616/660** | 1984 |
| after this pass | **4506 / 4506** | **93.4000** | **616/660** | 1984 |

No code change: analysis only. `check` after: `RATCHET PASS`, byte 4506/4524,
linked 4506/4524, size-exact 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, avg 99.94 %. Section deltas unchanged (`.text` 102,777 /
`.ARM.exidx` 842 / `.note.gnu.build-id` 20 differing bytes — identical to the
wave-123 snapshot). `git status --short` clean.

## 1. Exact residual of HEAD (re-measured this wave)

`SequenceMatcher` on the 660 normalised instructions: 616 matched, 44
unmatched per side (93.333 %). The 44 unmatched instructions are exactly:

| # | cluster | shipped | HEAD | sites |
|---|---|---|---|---|
| 1 | `i*3` register | `r4` | `r5` | `0x0dc 0x0e8 0x0fc 0x1ca 0x4a6 0x678 0x68c 0x6a8` (8) |
| 2 | `playerPosition` slot | `sp+44` | `sp+104` | `0x142` (1) |
| 3 | `delta` slot | `sp+104` | `sp+44` | `0x14c` (1) |
| 4 | `mov r6, rX` spill of `i*3` | `mov r6, r4` | `mov r6, r5` | `0x13c` (1) |
| 5 | post-`isAsteroid` reload | `ldr.w fp,[sp,#40]` | `add r4,sp,#104` | `0x208`, `0x2d2` (2) |
| 6 | `kNoAttribute` load | `ldr r0,[pc,#132]` | `ldr r0,[pc,#136]` + `ldr.w fp,[sp,#40]` | `0x2ea` (2) |
| 7 | `view` block register | `add r5,sp,#104` / `mov r0,r5` / `mov r1,r5` / `mov r2,r5` | `add r0,sp,#104` / `add r1,sp,#104` / `add r4,sp,#104` / `mov r2,r4` | `0x438`–`0x464` (5) |
| 8 | k-loop layout | rotated (`b.n` to header at `0x56c`; body at `0x4b2`) | header first at `0x4b0`, `bcs.w` exit, `b.n` back | `0x4b0`–`0x576` (8) |
| 9 | branch targets | — | shifted by −2 bytes from `0x1de` on (extra `add r4,sp,#104` at `0x208`) | rest |

`fp40` (count of `ldr.w fp,[sp,#40]`) is **5** in the shipped and **4** in
HEAD.

## 2. Correction to the wave-121/122/123 notes

The earlier notes state that HEAD transposes the `explodeOnContact` / `td`
spill slots (`sp+20`/`sp+24`). **That is wrong for HEAD.** The shipped and
HEAD prologues are byte-identical:

```
str r1, [sp, #16]   ; explodeOnContact
str r2, [sp, #20]   ; explosive
str r6, [sp, #24]   ; td
...
str r2, [sp, #28]   ; e
str r1, [sp, #32]   ; scatter
str r5, [sp, #36]   ; mine
str.w fp, [sp, #40] ; i
```

The transposition only appears in the `pgnew` / `B`-first families (§3), which
is why those variants read `ldr r6,[sp,#20]` instead of `ldr r6,[sp,#24]`.
So HEAD is *already correct* on the small-slot layout; the only structural
residuals are clusters 1–8 above.

## 3. New finding: the `expr` + B-first tail reproduces the shipped
## scatter block exactly

The shipped scatter block (`0x13c`–`0x15e`) is:

```
mov  r6, r4            ; spill i*3
blx  Level::getPlayer
add  r4, sp, #44       ; playerPosition
...
add  r5, sp, #104      ; delta
...
mov  r4, r6            ; restore i*3
```

i.e. `i*3 = r4`, `playerPosition = r4 / sp+44`, `delta = r5 / sp+104`, and
`i*3` shares `r4` with `playerPosition` across the block.

Combining the wave-123 expression form

```cpp
int distance = (int) AbyssEngine::AEMath::VectorLength(
        enemyPosition - ((Level *) this->level)->getPlayer()->getPosition());
```

with a tail that starts with the `playerGun` statement (`B` = playerGun,
`C` = setHitVector, `D` = lifetimes, `E` = lastHitKIPlayer, `A` = field_54):

```cpp
if (this->playerGun) { ((Level *) this->level)->field_30 = 1; }
enemy->setHitVector(-velocity.x, -velocity.y, -velocity.z);
this->lastHitKIPlayer = (int) (intptr_t) enemy->getKIPlayer();
reinterpret_cast<uint8_t *>(&enemy->field_54)[0] = 1;
this->lifetimes[i] = -1000000;
```

(`BCEAD`) gives **byte-identical `0x13c`–`0x15e`** — all four properties
(`i*3=r4`, `pp=r4/sp+44`, `delta=r5/sp+104`, `mov r6,r4`/`mov r4,r6`) — at
1984 B. `BECAD` does the same. Both score **90.8 %** because:

* the tail is emitted in `BCEAD` order (the shipped is `ABCDE`:
  `field54`, `playerGun`, `setHitVector`, `lifetimes`, `lastHitKIPlayer` —
  verified at shipped `0x153316`–`0x153350`), and
* the literal pool moves to `+0x358` (12 B earlier than the shipped `+0x364`),
  because the two `ldr.w fp,[sp,#40]` / `ldr r6,[sp,#24]` post-`isAsteroid`
  reloads are absent and the `movs r1,#1` / `strb.w r1,[r8,#84]` pair becomes
  `mov.w fp,#1` / `itt ne` / `ldrne` / `strbne`.

So the register/slot problem is *solvable*, but only by a tail reorder that
the shipped's own disassembly contradicts. `ABCDE` on the same `expr` base
gives `i*3=r5`, `pp=r5/sp+44` (92.6 %).

### 3.1 The 120-permutation tail sweep on the `expr` base

Full `A..E` permutation sweep (5! = 120 builds). Only two permutations reach
`i*3=r4`:

| order | pct | size | `i*3` | `pp` | pool |
|---|---:|---:|---|---|---:|
| `ABCDE` (= shipped order) | 92.6 | 1984 | r5 | r5/sp+44 | 0x364 |
| `ABDCE` | 92.2 | 1984 | r5 | r5/sp+44 | 0x364 |
| `ABCED` | 92.2 | 1984 | r5 | r5/sp+44 | 0x364 |
| `BCEAD` | **90.8** | 1984 | **r4** | **r4/sp+44** | 0x358 |
| `BECAD` | 90.7 | 1984 | **r4** | **r4/sp+44** | 0x358 |
| `BACDE` | 86.2 | 1984 | r5 | r5/sp+44 | 0x364 |
| `CABDE` | 91.7 | 1984 | r5 | r5/sp+44 | 0x364 |

(remaining 113 permutations: 86.0 % or lower, all `i*3=r5`). No `ABCDE`
permutation reaches `i*3=r4`; the shipped's own `ABCDE` tail and `i*3=r4`
therefore cannot both come from the `expr` scatter form.

## 4. Other levers tried this wave (all <= 93.4)

Format: `pct / size / i3 / pp / kloop / pool`.

* **Expression form** (wave-123 `expr`): 92.6 / 1984 / r5 / r5+44 / top /
  0x364. Scatter slots correct, registers swapped.
* `expr` + `__builtin_expect(scatter,1)` (`scatexp`): 86.0 / 1980 / r6 /
  r4+44 / top / 0x360. Slots and registers correct, `i*3=r6`.
* `expr` + `__builtin_expect(scatter,0)`: 72.6 / 1992 / r4 / – / top / 0x340.
* `expr` + `__builtin_expect(mine,0)`: 89.9 / 1984 / r6 / r4+44 / top.
* `expr` + `plaink` (drop cold k expect): 65.0 / 1908 / r0 / r5+104 / top.
* `expr` + `plaink` + `pgnew`: 86.6 / 1980 / r4 / r4+44 / rotated / 0x358.
* `expr` + `plaink` + `pgnew` + `isRocket` expect 0: 90.8 / 1984 / r4 /
  r4+44 / rotated / 0x358 (the wave-123 `E1K1P1S0_irk0` anchor, reproduced).
* `expr` + `plaink` + `pgnew` + `empDamage` expect 0: 90.1 / 1984.
* `expr` + `plaink` + `pgnew` + `scatexp`: 86.9 / 1984 / r6.
* **`expr` + `BCEAD`** / **`expr` + `BECAD`**: 90.8 / 1984 / r4 / r4+44 /
  top / 0x358 — scatter block byte-identical to the shipped (§3).
* `expr` + `BCEAD` + `isRocket` expect 0: 85.8 / 1988.
* `expr` + `BCEAD` + `empDamage` expect 0: 84.9 / 1988.
* `expr` + `BCEAD` + `plaink`: 86.7 / 1980 / rotated.
* k-loop forms on HEAD: `expect(distance < magnitude, 1)` -> 64.5 / 1908
  (RA collapse); `!expect(distance >= magnitude, 1)` -> 93.4 identical;
  `expect(magnitude > distance, 0)` -> 93.4 identical;
  `expect(!!(distance < magnitude), 0)` -> 93.4 identical;
  plain `if (distance < magnitude)` -> 64.5 / 1908; `do/while` -> 83.9 /
  1988. The cold `expect(...,0)` is what keeps the 1984 B size (without it the
  whole k-loop body is moved out of line and the function shrinks to 1908 B).
* scatter forms: `Vector playerPosition` statement form (HEAD) 93.4;
  `const Vector &playerPosition = …` 93.4; `Vector delta = enemyPosition -
  pp_expr` 92.6; `Vector delta; delta = …` 63.2; `Vector delta; Vector
  playerPosition; playerPosition = …; delta = …` 65.2; reversed operand
  order `playerPosition - enemyPosition` 93.1; `float len` 93.4;
  `{ Vector pp = …; distance = …; }` scoped block 93.4.
* `playerPosition` scope: i-loop top 93.4; before `enemyPosition` 93.4;
  function scope (statement form) 67.4 (i3=r4 but pp=sp+104); function scope
  (expression form) 92.6; assigned (not initialised) 82.8.
* `view` block: `Matrix view;` + assign 67.9 / 2056; `Matrix translated =
  MatrixSetTranslation(view,…)` 93.4 identical; `view = MatrixSetTranslation
  (view,…)` 87.0 / 1992; `Matrix view2 = view;` 93.4 identical; cached
  `Matrix *local = CameraGetLocal(...); Matrix view = *local;` 93.4
  identical; `Matrix translated;` before `view` 49.3 / 2016;
  `Matrix view = MatrixSetTranslation(CameraGetLocal(...), x,y,z)` (wave-123)
  81.4 / 1972 with a 160 B frame.
* `positions[i]` / `velocities[i]` instead of `.data()[i]`: 93.4 identical;
  cached `Vector *posData` / `*velData`: 93.1.
* declaration reorders in the i-loop body: velocity before position 89.1;
  `enemyPosition` first 86.5; `radius` declared uninitialised then assigned
  92.6; `radius = enemy->radius; if (useCustomRadius) …` 92.6.
* `ki` / `outer` / `r` hoisted to function scope: 92.6 identical; `ki` before
  the radius block: 59.3 / 1952.
* `td` as `Vector &`: 93.4 identical; `td` at function scope: 73.1 / 1964;
  `explodeOnContact` as `bool`: 93.4 identical; `explosive` as `bool`: 93.4
  identical; `explodeOnContact` declared before `explosive`: 86.6 on the
  `pgnew` base (no change on HEAD).
* `scatter`/`mine` as `int`: 92.6 identical; `scatter != 0` / `== 1` /
  `== true`: identical; `scatter` declared before `mine`: 91.4.
* `i` loop index: `int` 92.5; `!=` 92.5; `size_t` 92.6; cached count
  80.3 / 1988; `while` 58.9 / 1940; `e` as `while` 78.7 / 1992.
* `lifetimes[i] = -1000000.0f`: 93.4 identical; `lastHitKIPlayer = (int)
  enemy->getKIPlayer()`: 93.4 identical; `uint8_t *f54 = …; f54[0] = 1;`
  93.4 identical; `if (this->playerGun != 0)`: 93.4 identical;
  `hitPositions`/`hitFlags` swapped: 92.0; `hitPositions` moved after the
  k-loop: 59.0 / 1956; `impact` block before the `explosive` block: 84.8.

## 5. Root cause / conclusion

The shipped function needs, simultaneously:

1. `this=r9, enemy=r8, td=r6, i=fp` (HEAD has this),
2. `i*3=r4`,
3. `pp=r4/sp+44, delta=r5/sp+104`,
4. the k-loop rotated,
5. the two post-`isAsteroid` `ldr.w fp,[sp,#40]; ldr r6,[sp,#24]` reloads,
6. the literal pool at `+0x364` (1984 B).

(2)+(3) are reachable only through the expression form + a tail whose first
statement is `playerGun` (`BCEAD`/`BECAD`), which is *not* the shipped's
emitted tail order (`ABCDE`). That family also loses 12 B before the pool
(5) and scores 90.8 %. Every variant that keeps the shipped `ABCDE` tail
keeps `i*3=r5` (`pp=r5/sp+44` on the expression form, `pp=r4/sp+104` on the
statement form). The k-loop rotation (4) is coupled to the cold
`__builtin_expect(distance < magnitude, 0)`; dropping the expect rotates the
loop but collapses the register allocation to 1908 B.

Best measured variant remains **HEAD at 93.4000 % / 1984 B (616/660)** —
its instruction offsets are the shipped ones everywhere except the eight
clusters in §1, and the verifier compares intra-function branch targets
literally.

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

`expr` + `BCEAD` source shape (relative to HEAD `src/game/weapons/Gun.cpp`):

```cpp
// scatter distance: expression form (wave-123)
int distance = (int) AbyssEngine::AEMath::VectorLength(
        enemyPosition - ((Level *) this->level)->getPlayer()->getPosition());

// tail: playerGun first, then setHitVector, lastHitKIPlayer, field_54, lifetimes
if (this->playerGun) {
    ((Level *) this->level)->field_30 = 1;
}
enemy->setHitVector(-velocity.x, -velocity.y, -velocity.z);
this->lastHitKIPlayer = (int) (intptr_t) enemy->getKIPlayer();
reinterpret_cast<uint8_t *>(&enemy->field_54)[0] = 1;
this->lifetimes[i] = -1000000;
```

MIR: `clang++ … -mllvm -print-after-all -mllvm
-filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv` (the NDK clang is a
release build, so `-debug-only=regalloc` is unavailable; `-print-after-all`
still shows `Merge disjoint stack slots`, `Early Machine Loop Invariant Code
Motion`, `Greedy Register Allocator`, `Stack Slot Coloring` and
`Prologue/Epilogue Insertion & Frame Finalization`).
