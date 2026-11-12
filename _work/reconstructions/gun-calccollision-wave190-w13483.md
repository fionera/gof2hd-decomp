# `Gun::calcCharacterCollision` wave 190 (w13483): the scatter-block stack-slot
# swap is reproducible but does not move the `&view`/`i*3` register pair — no
# landed win

Worker: w13483. Branch `wave190-w13483`, base master `91ba24a04`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13483`.

## 0. Result

**No `src/` change landed.** The committed body remains the best measured
shape: **raw 455 / aligned 93 / 1984 B**, 660/660 insns.

| build | raw | aligned | size |
|---|---:|---:|---:|
| before (HEAD `91ba24a04`) | **455** | **93** | 1984/1984 |
| after this pass (note only) | **455** | **93** | 1984/1984 |

`git status --porcelain` is empty apart from this note. `link_so` + `fn_diff`
were re-run on the restored tree to confirm 455/93/1984.

## 1. New evidence: the scatter-block slot swap is reachable, and it is not enough

The residual is unchanged: 226 B of pure phase shift at
`0x153198..0x15327b` (two 2-B `add r4,sp,#104` remats where the original has
4-B `ldr.w fp,[sp,#40]`), 44 B in the view block (`0x1533c8..0x1533f5`), and
~167 B in the k-loop (`0x153440..0x153507`).

This pass found that the *scatter-block* stack-slot assignment can be made to
match the original exactly. The original has the `getPosition()` sret temp at
`sp+44` and the `enemyPosition - playerPosition` (`AEMath::mi`) sret temp at
`sp+104`; the committed source has them the other way round. Writing the
difference as a named value flips the frame-index order:

```c++
Vector diff = enemyPosition - ((Level *) this->level)->getPlayer()->getPosition();
int distance = (int) AbyssEngine::AEMath::VectorLength(diff);
```

now emits exactly the original's `add r5, sp, #44` (getPosition) /
`add r4, sp, #104` (mi) pair at `0x1530d2`/`0x1530dc`. But raw **rises to
459**: the frame-index-to-register map is unchanged (`%stack.10 -> r4`,
`%stack.11 -> r5` in both), so the four 1-byte register diffs in the scatter
block get worse and the 226-B region is untouched.

The mapping is a property of the *frame index*, not of instruction order. With
the committed source, `renamable $r4 = t2ADDri %stack.10` (playerPosition) and
`renamable $r5 = t2ADDri %stack.11` (diff); after the swap above it is still
`%stack.10 -> r4` / `%stack.11 -> r5` even though the two `t2ADDri` have
exchanged places in program order.

## 2. Register budget: no high register is free at the remat sites

From the post-RA machine dump (`-mllvm -print-after-all
-mllvm -filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv`, 13.6 MB):

| reg | holds |
|---|---|
| r4 | `&%stack.10` (view slot) |
| r5 | `i*3` |
| r6 | `td` / `i*3` save slot |
| r7 | frame pointer (`add r7, sp, #12`) |
| r8 | `enemy` |
| r9 | `this` |
| r10 (`sl`) | `&position` (`sp+192`) |
| r11 (`fp`) | `i` in the header / `radius` in the box check |

All eight callee-saved registers are occupied, so the hoisted `&%stack.10`
remat can only land in r4/r5 (`add rX, sp, #104`, 16-bit). The original uses
r5 for it and r4 for `i*3`; the swap is worth ~9 raw bytes
(`0x15306f`, `0x15307a`, `0x15308e`, `0x1530cc`, `0x153436`, `0x153438`,
`0x15360a`, `0x15361e`, `0x15363a`) but every source form tried that flips it
also moves the size off 1984 (w13471's `mag`-cache / radius-move /
`expect(isAsteroid,0)` cells).

### 2.1 Materialisation counts (ours 8, original 5)

`add rX, sp, #104` sites:

| site | original | ours |
|---|---|---|
| scatter block | `0x1530dc` r5 | `0x1530d2` r4 |
| asteroid block | — | `0x153198` r4 (dead remat) |
| non-asteroid block | — | `0x153260` r4 (dead remat) |
| view block | `0x1533c8` r5 (1x) | `0x1533ca` r0, `0x1533dc` r1, `0x1533ea` r4 (3x) |
| k-loop | `0x153444` r4 (inside loop) | — (uses r4 from `0x1533ea`) |
| mine block | `0x1535f8`, `0x153626` r6 | same |

So the original's `&%stack.10` live ranges are all short (scatter / view block
/ k-loop independently), while ours keeps one long range from the i-loop
preheader to the view block. `MachineLICM` hoists the rematerialisable
`t2ADDri` in both cases; the difference is entirely in greedy RA's region-split
(`RAGreedy::tryRegionSplit` / `splitAroundRegion`) — ours splits at bb.43 and
bb.55 (the two successors of the `isAsteroid` block) and leaves a live-through
child that is rematerialised at both block entries, the original does not.
`-mllvm -split-spill-mode=size` and `=default` both reproduce 455 exactly, so
the split decision is not `ComplementSpillMode`.

## 3. Cells measured this pass

All `link_so` + `tools/fn_diff.py`, `-j2`, `ulimit -v 12000000`. Size-exact
1984 unless stated; none beats 455.

### 3.1 Post-damage statement block — all 120 permutations

The five independent statements
`field_54=1`, `playerGun -> field_30=1`, `setHitVector`, `lifetimes[i]`,
`lastHitKIPlayer` were permuted exhaustively. The committed order
`f54-pgun-hitv-life-lastki` is the **unique** 455; the runner-up is
`f54-pgun-hitv-lastki-life` at 471, then 484, 494, 496, 496, 497, 504. Most
permutations are 1984 B; a few are 1964/1968/1988.

### 3.2 Big-block position

Moving the `if (explosive) … else if (geometries) { view }` block to each of
the six slots among the five statements: committed (last) is the unique 455;
the others are 1138–1379 (two are 1980 B). Moving the `if (impact) …` block
earlier is worse (best 718 at slot 4; 752/726 at slots 2/3).

### 3.3 View-slot forms

| variant | raw | aligned | size |
|---|---:|---:|---:|
| committed | **455** | **93** | 1984 |
| `*(const Matrix *)` cast | 455 | 93 | 1984 |
| `*reinterpret_cast<Matrix *>(…)` | 455 | 93 | 1984 |
| `Matrix *cam = …; Matrix view = *cam;` | 455 | 93 | 1984 |
| `const Matrix &cam = *(Matrix *)…; Matrix view = cam;` | 455 | 93 | 1984 |
| `Matrix tmp = MatrixSetTranslation(…); (void)tmp;` | 455 | 93 | 1984 |
| `Matrix view; __builtin_memcpy(&view, …, sizeof view)` | 1919 | 277 | 2064 |
| `static Matrix view; view = *(Matrix *)…` | 1725 | 265 | 2056 |
| `static Matrix view = *(Matrix *)…` | 1762 | 274 | 2048 |
| `unsigned cam = CameraGetCurrent(); …CameraGetLocal(cam)` | 1446 | 306 | 1968 |
| `Matrix view;` before the `if (explosive)` + assign | 1886 | 409 | 2040 |
| `Matrix view = …` before the `if (explosive)` | 1821 | 315 | 1964 |
| `Matrix view;` at i-loop body top + assign | 1880 | 393 | 2024 |
| `Matrix view;` at i-loop body top + memcpy | 1870 | 393 | 2024 |
| `Matrix view = …` at i-loop body top | 1876 | 482 | 1904 |
| `Matrix view;` at function scope + assign | 1885 | 426 | 1960 |
| `Matrix view;` at function scope + memcpy | 1872 | 455 | 1968 |
| `view = MatrixSetTranslation(view, …)` | 1103 | 137 | 1992 |
| `Matrix tmp = MatrixSetTranslation(…); TransformSetLocal(…, tmp)` | 1255 | 175 | 1988 |

### 3.4 Loop / pointer / declaration forms

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `i = i + 1` in either loop (all three) | 455 | 93 | 1984 |
| `for (unsigned i = 0, i3 = 0; …; ++i, i3 += 3)` | 455 | 93 | 1984 |
| `unsigned i3 = i * 3;` in the body | 455 | 93 | 1984 |
| `Vector *positionp = &positions[i]; Vector position = *positionp;` | 455 | 93 | 1984 |
| `td` → `Vector &td` / removed (use `this->targetDir`) / declared in scatter block | 455 | 93 | 1984 |
| `Vector diff = enemyPosition - getPosition()` (slot swap, §1) | 459 | 103 | 1984 |
| `Vector playerPosition = …; Vector diff = enemyPosition - playerPosition;` | 455 | 93 | 1984 |
| `*td -= this->positions.data()[i]` | 1812 | 284 | 1932 |
| `hitPositions[i] = this->positions.data()[i]` | 1059 | 130 | 1988 |
| `const Vector &position = positions[i]` | 1812 | 350 | 1912 |
| `const Vector &velocity = velocities[i]` | 1665 | 357 | 1968 |
| cache `this->enemies` in a local `Array<Player*> *` | 510 | 98 | 1984 |
| `int radius` computed per branch (declared late) | 1100 | 155 | 1984 |
| split `if (mine)` from `else if (scatter)` | 1803 | 275 | 1976 |

### 3.5 `expect` placements (new ones)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `expect(falloff > 1.0f, 0)` on the clamp | **455** | **93** | 1984 |
| `expect(falloff > 1.0f, 1)` | 928 | 125 | 1984 |
| `expect(owner == nullptr \|\| owner->enemyFlagsLo, 1)` | 533 | 112 | 1984 |
| same, `0` | 554 | 125 | 1984 |
| `expect(geometries != nullptr, {1,0})` | 1608 / 1378 | 265 / 379 | 1956 / 1972 |
| `expect(mine, 0)` | 1049 | 157 | 1984 |
| `expect(scatter, 0)` on the radius branch | 1828 | 311 | 1992 |
| `expect(scatter, 0)` on the hit block | 1760 | 371 | 1936 |
| `expect(!(outer && outerCollide), {0,1})` | 1821 / 1720 | 687 / 480 | 1952 / 1952 |
| `expect(impact != nullptr, 0)` | 1079 | 147 | 1992 |
| `expect(k < size, {1,0})` in the k-loop header | 1798 / 1710 | 379 / 341 | 1948 / 1944 |

Two forms are **codegen-identical** to the committed body and can be used as
free rewrites: `if (!outer || !ki->outerCollide(position))` (De Morgan) and
`expect(falloff > 1.0f, 0)`.

## 4. Conclusion

The residual remains the greedy-RA plateau characterised by w13449/w13471:

1. The 226-B phase shift needs the hoisted `&%stack.10` remat to be 4 B. With
   r4–r11 all live (r7 is the frame pointer), the only 4-B form would be a
   high-register `add.w`, and no source form frees one without moving the
   size off 1984.
2. The scatter block's stack-slot assignment *can* be made to match the
   original (`Vector diff = enemyPosition - getPosition()`), but the
   frame-index→register map (`%stack.10 -> r4`, `%stack.11 -> r5`) is a
   property of the frame index, not of instruction order, and does not move.
3. The post-damage statement order, the view/impact block positions, the
   k-loop body order and the committed `expect(diff > 0, 0)` size anchor are
   all already the unique optimum among their permutation spaces.

The committed **455 / 93 / 1984** body remains the best measured shape.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13483
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
# -> raw=455 aligned=93 size=1984/1984 insn=660/660
```

## 6. Files changed

`_work/reconstructions/gun-calccollision-wave190-w13483.md` only.
