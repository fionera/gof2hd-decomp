# `Gun::calcCharacterCollision` wave 190 (w13445): the residual is one greedy-RA
# decision (`i*3` r5 vs `&view` r4); no landed win

Worker: w13445. Branch `wave190-w13445`, base master `f0f4d7918`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13445`.

## 0. Result

**No `src/` change landed.** The committed body (wave-190 w13388/w13406/w13417)
remains the best measured shape: **raw 455 / aligned 93 / 1984 B**, 660/660 insns.
Tree clean apart from this note.

| build | raw | aligned | size |
|---|---:|---:|---:|
| before (HEAD `f0f4d7918`) | **455** | **93** | 1984/1984 |
| after this pass (note only) | **455** | **93** | 1984/1984 |

Gate on entry/exit unchanged: `check` green (byte_exact 4507/4524, linked_exact
4507/4524, size-exact 4524/4524, stub_zero_size 0, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370, `.text` diff 72,849).

## 1. Re-confirmed decomposition (committed vs orig, 93 aligned diffs)

`fn_diff` categories: `operand:cbz 11`, `regalloc:add.w 7`, `operand:b.n 3`,
`operand:ldr 2`, `operand:vldr 2`, `regalloc:mov 2`, `opcode:ldr.w/r4 2`,
`extra-ours:ldr.w 2`.  Every one is downstream of a single register swap:

| value | orig | ours |
|---|---|---|
| `i*3` (`add.w R, fp, fp, lsl #1`) | **r4** | r5 |
| `&view` / scatter temp / k-loop sret slot (sp+104) | **r5** | r4 |
| `playerPosition` (sp+44) | **r4** | r5 |

`i*3` and `&view` overlap (153198..153444), so greedy RA gives one r4 and the
other r5; ours gives the long hoisted `t2ADDri %stack.10` the lower register.
Consequences, all measured:

- `0x153198`/`0x153262`: orig reloads `i` (`ldr.w fp,[sp,#40]`, 4 B); ours remats
  `&view` (`add r4,sp,#104`, 2 B).  Two 2-byte shortenings → cumulative phase
  shift -2/-4 over `0x153198..0x15327b` = **226 raw bytes**.
- `0x1533c8..0x1533f5` = **44 raw bytes**: orig keeps sp+104 in r5 across
  memcpy/MatrixSetTranslation/TransformSetLocal; ours remats it 3× because r5
  holds `i*3`.
- `0x153440..0x153507` = **~167 raw bytes**: ours top-tests the k-loop (cold
  then-block from `expect(diff>0,0)`); orig rotates it with the then-block as
  fallthrough.  Ours has 1 extra instruction in the k-loop (`b.n` latch, 2 B),
  compensated elsewhere; totals are 660/660 insns and 1984/1984 B.
- `0x15306f`,`0x15307a`,`0x15308e`,`0x15360a`,`0x15361e`,`0x15363a`: 1-byte
  `i*3` r5-vs-r4 regalloc bytes.

Literal pool and prologue are byte-identical; the tail from `0x1536d0` is
byte-identical.  The stack layout is identical (view sp+104, MatrixSetTranslation
return temp sp+44, playerPosition sp+44, enemyPosition sp+164, velocity sp+176,
position sp+192, canary sp+204, frame 208).

## 2. New cells measured this pass (all `link_so` + `tools/fn_diff.py`, `-j2`)

All size-exact (1984) unless stated; none beats 455.

### 2.1 `continue` / `else` forms of the k-loop guard

| variant | raw | aligned | size |
|---|---:|---:|---:|
| committed (`if (expect(diff>0,0)) {body}`) | **455** | **93** | 1984 |
| `if (expect(diff<=0,1)) continue;` + body | 455 | 93 | 1984 |
| `if (!expect(diff>0,0)) continue;` + body | 455 | 93 | 1984 |
| `if (expect(diff<=0,0)) continue;` + body | 1887 | 281 | 1908 |
| `if (expect(diff<=0,1)) {} else {body}` | 455 | 93 | 1984 |
| `if (expect(diff<=0,0)) {} else {body}` | 1887 | 281 | 1908 |
| `then-continue` / `else-continue` (redundant) | 455 | 93 | 1984 |
| `then-break` | 1781 | 199 | 1968 |

### 2.2 Diff-expect = 1 plus a second cold anchor (46 builds)

`expect(diff>0,1)` alone = 1887/1908.  Adding `expect(C,{0,1})` for 23 conditions
never produced 1984; best raw at any size was `geom` 0 = 1310/1968.  Selected
size-1984 cells: `explosive`1 1441/1972, `scatter`1 1820/1888, `eoc`1 1790/2000,
`outer`1 1736/1952, `kloop`0 1714/1924, `kloop`1 1867/1904.  Confirms the
wave-190 w13417 finding that `expect(diff>0,0)` is the unique 1984-B anchor.

### 2.3 k-loop body order / clamp form

| variant | raw | aligned | size |
|---|---:|---:|---:|
| committed E-C-D | **455** | **93** | 1984 |
| C-D-E (faithful) | 458 | 63 | 1984 |
| C-D-E, clamp `<0` first | 460 | 71 | 1984 |
| C-E-D | 457 | 84 | 1984 |
| C-E-D, clamp `<0` first | 459 | 92 | 1984 |
| E-C-D, clamp `<0`/`<=0`/`0.0f>f` first | 456 | 94 | 1984 |
| E-C-D, clamp `>1`/`>=1`/`1.0f<f` first | 455 | 93 | 1984 |
| two separate clamp `if`s (any order) | 1019..1021 | 131..134 | 1960 |
| split clamp (one `if` before EMP, one after) | 1020 | 133 | 1960 |
| E-C-D with `damage` before `damageEmp` | 457 | 84 | 1984 |
| E-D-C | 1794 | 171 | 1940 |

### 2.4 View block / `view` lifetime (goal: force the k-loop sret temp off sp+104)

| variant | raw | aligned | size | frame |
|---|---:|---:|---:|---:|
| committed | **455** | **93** | 1984 | 208 |
| `Matrix view;` at i-loop scope, `view = *(...)` (operator=) | 1875 | 367 | 2024 | 232 |
| same at e-loop scope | 1879 | 377 | 2024 | 232 |
| same at function scope | 1885 | 426 | 1960 | — |
| `Matrix view;` at i-loop scope + explicit `__builtin_memcpy` | 1866 | 367 | 2024 | 232 |
| same at e-loop / function scope | 1869 / 1872 | 377 / 455 | 2024 / 1968 | — |
| `char viewbuf[60]` + memcpy in block | 455 | 93 | 1984 | 208 |
| `char viewbuf[60]` at i-loop scope + memcpy | 1804 | 354 | 1948 | 224 |
| `Matrix viewbuf[1]` at i-loop scope | 1868 | 478 | 1896 | — |
| `char viewbuf[64]` + `viewbuf+{1,2,3,6}` | 1861 | 387 | 1896 | — |
| `Matrix *const volatile viewp = &view` | 1128 | 174 | **1988** | 208 |
| `Matrix *viewp = &view` / `Matrix &vr = view` | 455 | 93 | 1984 | 208 |
| `float *cam = CameraGetLocal(...)` cached | 455 | 93 | 1984 | 208 |
| `int cami = CameraGetCurrent()` cached | 1446 | 306 | 1968 | — |
| explicit `Matrix view; __builtin_memcpy(&view,...)` | 1919 | 276 | 2064 | — |
| `Matrix view = MatrixSetTranslation(*cam, x,y,z)` (copy-elided) | 1167 | 177 | 1972 | — |
| same, Vector overload | 1208 | 179 | 1960 | — |
| `AbyssEngine::Transform *tr` cached (2 calls → 1) | 1174 | 142 | 1968 | — |
| `AbyssEngine::PaintCanvas *canvas` cached | 487 | 133 | 1984 | 208 |

The `Matrix view` slot (sp+104) and the `MatrixSetTranslation` return temp
(sp+44, 60 B) are the two 60-byte slots; the k-loop `Vector` sret temp (12 B,
`%13`) is stack-colored into the view slot because their lifetimes are disjoint.
Every attempt to lengthen `view`'s lifetime or to allocate a separate k-loop
temp either folds back to 455 or grows the frame (208 → 224/232), which shifts
every sp-relative access and dominates the raw metric.

### 2.5 k-loop sret temp / `td` forms

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `enemyPosition = enemy->getPosition(); ...` in k-loop | 1731 | 305 | 1940 |
| `velocity = enemy->getPosition(); ...` | 1731 | 311 | 1948 |
| `position = enemy->getPosition(); ...` | 1739 | 326 | 1920 |
| `*td -= this->positions.data()[i]` | 1812 | 284 | 1932 |
| `Vector d = enemyPosition - playerPosition` in scatter | 455 | 93 | 1984 |
| `Vector d = enemyPosition; d -= playerPosition` | 1553 | 229 | 1992 |
| scatter `distance` inlined (`enemyPosition - getPlayer()->getPosition()`) | 459 | 103 | 1984 |
| scatter `playerPosition - enemyPosition` | 457 | 95 | 1984 |
| `unsigned geom = geometries[i]` cached in view block | 1212 | 224 | 1972 |
| `Array<Player*> *enemies` cached at i-loop | 455 | 93 | 1984 |

### 2.6 Block reorder (explosive/geometries = EXP, impact = IMP, scatter = SCA)

| order | raw | aligned | size |
|---|---:|---:|---:|
| EXP-IMP-SCA (committed) | **455** | **93** | 1984 |
| IMP-EXP-SCA | 1221 | 159 | **1984** |
| EXP-SCA-IMP | 1784 | 251 | 1972 |
| IMP-SCA-EXP | 1814 | 387 | 1964 |
| SCA-EXP-IMP | 1895 | 379 | 1904 |
| SCA-IMP-EXP | 1905 | 392 | 1900 |
| SCA after `explodeOnContact` / before EMP | 1693 | 461 | 1948 |
| SCA before `setHitVector` | 1888 | 381 | 1900 |

`IMP-EXP-SCA` keeps 1984 B but the phase shift and the tail are worse (1221).

### 2.7 Type / cast / statement micro-variants (all 455/93/1984)

`bool explosive`, `int mine`, `int scatter`, `int bomb`, `bool explodeOnContact`,
non-const `weaponType`, `Vector *const td`, `int radius` via if, radius casts,
`(float)` casts on `diff`/`magnitude`/`damage`/`empDamage`, multiplication
operand order, `-distance + magnitude`, `(int)distance`, `const float falloff`,
`int mag` local, `int emp`/`int dmg` cached before the guard (457/1010),
`#pragma`-free redundant `continue`s, `hitFlags`/`hitPositions` order.

## 3. Conclusion

The whole 455-byte residual is one greedy-RA decision: the hoisted
`t2ADDri %stack.10` (address of the shared `view`/scatter/k-loop temp slot,
sp+104) is assigned r4, pushing `i*3` to r5.  The 2-byte `add r4,sp,#104`
remat versus the shipped 4-byte `ldr.w fp,[sp,#40]` reload then produces the
226-byte phase shift, the 44-byte view-block remat, and the 167-byte k-loop
layout.  The `expect(diff>0,0)` that pins the 1984-byte size is also what keeps
the k-loop top-tested (the original rotates it with a hot then-block), so the
size anchor and the layout cannot be decoupled by any source form measured here.
The committed **455 / 93 / 1984** body remains the best measured shape.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13445
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
# -> raw=455 aligned=93 size=1984/1984 insn=660/660
```

## 5. Files changed

`_work/reconstructions/gun-calccollision-wave190-w13445.md` only.
