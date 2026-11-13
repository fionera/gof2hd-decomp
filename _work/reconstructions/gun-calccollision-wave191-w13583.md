# `Gun::calcCharacterCollision` wave 191 (w13583): the k-loop layout *is*
# reachable with the committed RA (increment-at-top forms), but every such
# form is either 1980 B or > 455 raw — no landed win

Worker: w13583. Branch `wave191b-w13583`, base master `e884074af`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13583`.

## 0. Result

**No `src/` change landed.** The committed body remains the best measured
shape: **raw 455 / aligned 93 / 1984 B**, 660/660 insns.

| build | raw | aligned | size | insns |
|---|---:|---:|---:|---:|
| before (HEAD `e884074af`) | **455** | **93** | 1984/1984 | 660/660 |
| after this pass (note only) | **455** | **93** | 1984/1984 | 660/660 |

`git status --porcelain` is empty apart from this note.

## 1. New evidence: the k-loop block order is **not** downstream of the RA

w13564 §2 concluded "the k-loop layout cannot be fixed by a source-level loop
rewrite alone while the RA stays the committed one; it is downstream of the
RA." **That is wrong.** Moving the `++k` from the `for` increment into the
body makes the loop rotate (body-first, header at the bottom, `bcc` back to
the body) **while the committed register allocation is completely unchanged**
(`this`=r9, `enemy`=r8, `i`=fp spilled at `sp+40`, `td`=r6, `%stack.10`=r4,
`i*3`=r5, `&position`=sl, 8 `add rX,sp,#104` sites — identical to the
committed build).

The k-loop block order is therefore a pure *source* lever (the latch block
must exist and be hot), exactly like w13553 §3's `expect` placement lever —
but the two levers are independent, and the "increment at the top" form gets
the rotation *without* giving up the 1984-B spill-heavy RA.

## 2. The two body-first forms and why neither wins

### 2.1 `while (k < size) { ++k; ... }` — body-first, **1984 B**, raw 1248

```c++
unsigned k = 0;
while (k < this->enemies->size()) {
    ++k;
    enemy = (*this->enemies)[k];
    ...
}
```
Rotates to exactly the original's block order (`movs r5,#0; b.n header;
body; latch; header; bcc body; b exit`) and stays **1984 B / 660 insns** with
the committed RA. It loses on raw because `++k` at the top makes the enemy
load `data[k+1]`, which ARM cannot fold into `ldr r8,[r0,r5,lsl#2]` and
instead costs an extra `add.w r0,r0,r5,lsl#2` (4 B), and because the
`adds r5,#1` is scheduled into the middle of the body (`0x15346e`) instead of
the latch.

### 2.2 `while (k < size) { enemy = ...; ++k; ... }` — body-first, 1980 B

Moving the `++k` one statement later restores the exact enemy load
(`ldr.w r8,[r0,r5,lsl#2]`, no extra `add`) but the function drops to
**1980 B / 659 insns**: the k-loop no longer rematerialises `&stack.10`
(`add r4,sp,#104` is missing; r4 already holds it) and the literal pool loses
one word.

Combined with the faithful C-D-E body plus a named `Vector kpos`, this is the
closest body-first shape measured:

| variant | raw | aligned | size | insns | k-loop |
|---|---:|---:|---:|---:|---|
| committed (E-C-D, `expect(diff>0,0)`) | **455** | **93** | 1984 | 660 | header-first |
| faithful (C-D-E, `expect(mag>dist,0)`) | 463 | **57** | 1984 | 660 | header-first |
| `while { ++k; ... }`, committed body | 1248 | 143 | **1984** | 660 | **body-first** |
| `while { ++k; ... }`, faithful body | 1147 | 100 | **1984** | 660 | **body-first** |
| `while { enemy=..; ++k; .. }`, committed body | 1585 | 161 | 1980 | 659 | **body-first** |
| `while { enemy=..; ++k; .. }`, faithful body | 1577 | 121 | 1980 | 659 | **body-first** |
| `while { enemy=..; ++k; .. }`, faithful + `Vector kpos` | **920** | **61** | 1980 | 659 | **body-first** |

The last row is the best body-first shape by a wide margin (raw 920 vs 1147
for the next best). Its k-loop body is instruction-for-instruction identical
to the original except (a) the missing `add r4,sp,#104` remat and (b) the
`adds r5,#1` sitting at `0x15346e` instead of the latch — the relocation
shifts the remaining ~140 B of body and the whole mine block by 4 B, which is
where most of the 920 comes from.

Every attempt to recover the missing 4 B (alignas(8)/alignas(16) `kpos`,
`Vector kpos` at i-loop / function scope, `int k`, `k += 1`, `k = k + 1`,
extra `++k; --k;`, `Vector &kref`) is codegen-identical (920/61/1980) or
overshoots (1992 B).

## 3. Cells measured this pass (all `link_so` + `tools/fn_diff.py`, `-j2`)

Size-exact 1984 unless stated; none beats 455.

### 3.1 increment-at-top / body-first family

| variant | raw | aligned | size | insns |
|---|---:|---:|---:|---:|
| committed | 455 | 93 | 1984 | 660 |
| `while { ++k; ... }` (A) | 1248 | 143 | 1984 | 660 |
| `while { ++k; data[k-1] }` | 1585 | 161 | 1980 | 659 |
| `while { ++k; ... }` faithful (J) | 1147 | 100 | 1984 | 660 |
| `while { ++k; data[k-1] }` faithful | 1577 | 121 | 1980 | 659 |
| `for (...; ) { ++k; data[k-1] }` | 1585 | 161 | 1980 | 659 |
| `while { enemy=..; ++k; .. }` | 1585 | 161 | 1980 | 659 |
| `while { enemy=..; ++k; .. }` faithful | 1577 | 121 | 1980 | 659 |
| `while { enemy=..; ++k; .. }` faithful + `kpos` | **920** | **61** | 1980 | 659 |
| `int k=-1; while (++k < size)` | 455 | 93 | 1984 | 660 (header-first) |
| `int k=-1; while (++k < size)` faithful | 463 | 58 | 1984 | 660 (header-first) |
| `unsigned k=-1; while (k<size) { ++k; }` | 1891 | 345 | 1668 | 581 |
| `do { ...; ++k; } while (k<size)` | 1027 | 156 | 1988 | 662 |

### 3.2 stack-slot / lifetime interference (aimed at the `%stack.10` remat)

| variant | raw | aligned | size | insns |
|---|---:|---:|---:|---:|
| `Matrix view;` at i-loop scope + assign | 1877 | 394 | 2024 | 682 |
| `Vector playerPosition;` at i-loop scope | 1606 | 309 | 1964 | 661 |
| both at i-loop scope | 1907 | 405 | 2048 | 692 |
| `Matrix view;` i-loop scope + `Vector kpos;` i-loop scope | 1759 | 487 | 1992 | 687 |
| `Matrix view;` fn scope + `playerPosition` i-loop scope | 1938 | 371 | 2032 | 685 |
| `Vector kpos;` at i-loop scope + `kpos = getPosition()` | 1742 | 323 | 1976 | 666 |
| same, faithful body | 1734 | 299 | 1976 | 666 |
| `Vector kpos;` at function scope | 1867 | 310 | 1972 | 664 |
| `kpos = getPosition(); kpos -= position; *td = kpos;` | 1855 | 325 | 1916 | 662 |
| reuse `velocity` as the k-loop sret | 1802 | 308 | 1964 | 661 |
| reuse `enemyPosition` as the k-loop sret | 1739 | 202 | 1992 | 667 |
| `Vector diff = enemyPosition - getPosition()` (w13483 slot swap) | 459 | 103 | 1984 | 660 |
| `MatrixSetTranslation(view, position)` (Vector overload) | 1157 | 139 | 1972 | 657 |
| `view = MatrixSetTranslation(view, ...)` | 1103 | 137 | 1992 | 663 |
| `Matrix tmp = MatrixSetTranslation(...); TransformSetLocal(..., tmp)` | 1255 | 175 | 1988 | 662 |
| static helper `setGeometryView(geom, position)` (inlined) | 1170 | 218 | 1988 | 662 |
| static helper `scatterHit(...)` for the k-loop body (inlined) | 1809 | 495 | 1964 | 678 |

### 3.3 statement order / declarations / types (committed base)

All codegen-identical 455/93/1984: `lastHitKIPlayer` after `td`; `td` inside
the e-loop; `explodeOnContact` declared first; `explosive` declared at the
assignment; `bomb` without the extra parens; `enemies->data()[e]`;
`getHitpoints() <= 0`; `isActive() == 0`; `!outer || !outerCollide(...)`
(De Morgan).

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `scatter` declared before `mine` | 487 | 108 | 1984 |
| `if (!mine) { ... }` instead of `if (mine) continue;` | 1735 | 453 | 1956 |
| cache `enemies->size()` for the e-loop | 510 | 98 | 1984 |
| cache `enemies->size()` for the k-loop (`kn`) | 1714 | 306 | 1960 |
| cache `this->enemies` for the k-loop (`ken`) | 1742 | 348 | 1952 |

### 3.4 `__builtin_expect_with_probability`

**Not available in clang 7.0.2** (`use of undeclared identifier`). Only the
two-valued `__builtin_expect` lever exists.

## 4. Conclusion

The residual is unchanged and still one greedy-RA decision (`%stack.10` in
r4 vs `i*3` in r4). What this pass adds:

1. The k-loop **block order** is a genuine independent source lever: the
   `increment-at-top` loop forms rotate the loop to the original's
   body-first layout **with the committed RA intact** (contradicting
   w13564 §2). They are the only 1984-B body-first shapes found.
2. That lever costs more than it saves: the increment relocation shifts the
   body by 4 B, and the best body-first shape (`iae-faithful-kpos`) is
   1980 B / raw 920, i.e. 4 B short of size-exact and 465 raw bytes worse
   than the committed body.
3. The `%stack.10` remat (`add r4,sp,#104` at the k-loop) is still absent in
   every body-first shape; separating the k-loop sret from the `view` /
   `playerPosition` slot by lifetime interference always moves the frame size
   off 1984 (1964–2048).

The committed `expect(diff > 0, 0)` + E-C-D body remains the unique optimum.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13583
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
# -> raw=455 aligned=93 size=1984/1984 insn=660/660
```

The body-first cell (§2.2, last row):

```c++
unsigned k = 0;
while (k < this->enemies->size()) {
    enemy = (*this->enemies)[k];
    ++k;
    Vector kpos = enemy->getPosition();
    *td = kpos;
    *td -= position;
    int distance = (int) AbyssEngine::AEMath::VectorLength(*td);
    if (__builtin_expect(this->magnitude > distance, 0)) {  // lint: arm_builtin
        int diff = this->magnitude - distance;
        float falloff = (float) diff / (float) this->magnitude;
        if (falloff > 1.0f) falloff = 1.0f;
        else if (falloff < 0.0f) falloff = 0.0f;
        enemy->damage((int) (falloff * (float) this->damage), this->friendGun, this->itemIndex);
        if (this->empDamage > 0) {
            enemy->damageEmp((int) (falloff * (float) this->empDamage), this->friendGun);
        }
    }
}
# -> raw=920 aligned=61 size=1980/1984 insn=659/660, k-loop body-first
```

## 6. Files changed

`_work/reconstructions/gun-calccollision-wave191-w13583.md` only.
