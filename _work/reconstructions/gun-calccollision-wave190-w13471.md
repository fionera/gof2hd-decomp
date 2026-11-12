# `Gun::calcCharacterCollision` wave 190 (w13471): the phase shift is driven by
# the *width* of the hoisted `&view` remat, not by the `i*3` register — no
# landed win

Worker: w13471. Branch `wave190-w13471`, base master `99a8383ce`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13471`.

## 0. Result

**No `src/` change landed.** The committed body remains the best measured
shape: **raw 455 / aligned 93 / 1984 B**, 660/660 insns. Tree clean apart from
this note.

| build | raw | aligned | size |
|---|---:|---:|---:|
| before (HEAD `99a8383ce`) | **455** | **93** | 1984/1984 |
| after this pass (note only) | **455** | **93** | 1984/1984 |

Gate on exit: `check` green (byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370).

## 1. New evidence: the 226-B phase shift is an instruction-*width* effect

The previous wave-190 notes (w13417/w13432/w13445/w13449/w13463) describe the
residual as the greedy-RA `i*3 -> r5` / `&view -> r4` swap. This pass shows
that the *swap is not the phase-shift cause*: several plain-source forms flip
`i*3` back to `r4` while the 226-byte region is untouched, because the hoisted
`&view` remat stays in a **low** register and therefore stays a 2-byte
`add r4, sp, #104` where the original has a 4-byte `ldr.w fp, [sp, #40]`.

The phase shift is fixed only if the remat at `0x153198`/`0x153260` becomes a
4-byte encoding (i.e. a high register `r8..r11` -> `add.w`). The greedy
allocator always picks `r4`/`r5` there because nothing else is live in that
slot.

### 1.1 Forms that flip `i*3` to `r4` but leave the remat 2 bytes

All measured with `link_so` + `tools/fn_diff.py` (`-j2`,
`ulimit -v 12000000`). `i3` is the register of the `add.w R, fp, fp, lsl #1`
loop-index multiply.

| variant | raw | aligned | size | i3 | note |
|---|---:|---:|---:|---|---|
| committed | **455** | **93** | 1984 | r5 | |
| `const int mag = this->magnitude;` before e-loop, `diff = mag - distance` | 1459* | — | 1960 | **r4** | `this` -> r8, `i` -> r1 |
| same, before the k-loop | 1481* | — | 1952 | **r4** | |
| same, in the i-loop body | 1551* | — | 1956 | **r4** | |
| radius block moved after `ki`/egoFlag check | 1538* | — | 1952 | **r4** | |
| radius block moved after `bool outer = ...` | 1331* | — | 1964 | **r4** | |
| `expect(isAsteroid(),0)` (w13417) | 1813 | 376 | 1980 | r4 | `this` -> fp |

(*proxy raw; bl/blx masked. The committed proxy is 436, official raw 455.)

In every one of these the `&view` remat is still `add r5, sp, #104` /
`add r4, sp, #104` (2 B), so the phase shift survives; the size also moves off
1984.

### 1.2 The rot at `0x153440` is a separate, size-anchored effect

`for(;;)` + `if (++k == size) break;` (or `do/while`, or `while(true)`) does
produce the shipped bottom-tested k-loop, but it is **1988 B** and places the
cold `diff>0` then-block *out of line before the loop body* (the original has
it inline as the `ble` fall-through). `expect(diff>0,0)` is still the unique
1984-B anchor; `expect(diff>0,1)`/plain is 1908 B and re-colours `this` to r4.

## 2. New cells measured this pass (all size-exact 1984 unless stated)

### 2.1 i-loop prefix order (24 permutations of position/velocity/enemyPosition/radius)

`PVER` (committed) is uniquely best at proxy **436**; next `VPER` 451,
`PEVR` 477, `PVRE` 486, `REPV` 491. `radius`-first forms drop to 1960/1968.
No permutation moves `i3` off r5.

### 2.2 k-loop body order (E=emp, C=clamp, D=damage), committed condition

| order | proxy | size |
|---|---:|---:|
| ECD (committed) | **436** | 1984 |
| CED | 438 | 1984 |
| CDE (faithful, matches orig disasm order) | 439 | 1984 |
| DCE | 444 | 1984 |
| EDC / DEC | 1525 / 1535 | 1940 |

`magnitude > distance` + diff-inside (the `cmp r0,r1` form) is 441 (ECD),
443 (CED), 444 (CDE), 447 (DCE) — all worse than 436.

### 2.3 Other statement/declaration cells

| variant | proxy | size |
|---|---:|---:|
| `if (falloff < 0) ... else if (> 1)` (clamp reversed) | 437 | 1984 |
| `lifetimes[i]` before `setHitVector` | 461 | 1984 |
| `playerGun` block before `field_54` | 1026 | 1984 |
| `owner` block before `empDamage` | 1196 | 1976 |
| `for`-condition `expect(i < count, {0,1})` | 436 | 1984 (identical) |
| `for`-condition `expect(e < size, {0,1})` | 436 | 1984 (identical) |
| `expect(enemies == nullptr, 0)` | 436 | 1984 (identical) |
| `expect(!enemyFlagsLo, 0)` | 436 | 1984 (identical) |
| `expect(!isActive(), 0)` / `expect(hp < 1, 0)` | 616 / 615 | 1984 |
| `Vector *pp/vp` cached before e-loop (unused) | 436 | 1984 |
| `Vector *pp/vp` cached + used | 1523 | 1972 |
| `Matrix *td` / `Vector &td` / `Vector *const td` | 436 | 1984 (identical) |
| `*(data()+i)` / `hitFlags+i` / `geometries+i` | 436 | 1984 (identical) |

### 2.4 View-slot forms

| variant | proxy | size | i3 |
|---|---:|---:|---|
| packed `struct { char pad[N]; Matrix m; }` (N=1..7) | 1602 | 1976 | r6 |
| `Matrix viewbuf; Matrix &view = *(Matrix*)((char*)&viewbuf + (i>>31))` + memcpy | 1697 | 2072 | r5 |
| `Matrix view; Matrix &ref = *(Matrix*)((char*)&view + (i>>31))` | 977 | 1992 | r5 |
| `Matrix view[1]` + memcpy | 1028 | 1988 | r5 |
| `Matrix *volatile vp = &view` | 1680 | 2048 | r5 |

`(i>>31)` is the cheapest non-foldable zero (clang 7 range analysis folds
`i&0`, `i*0`, `i-i`, `e&1`, `e>>31`), but it costs >= 8 B and shifts the body.

### 2.5 Random 1–3 knob sweeps

Two independent random sweeps (2500 + 1500 variants over 31 statement /
declaration / `expect` knobs, 4-way parallel `-j2`) found **no size-1984
variant with proxy < 436**. The committed body is the unique floor.

## 3. Conclusion

The residual is a genuine greedy-RA plateau with a sharper characterisation
than before:

1. The 226-B phase shift needs the hoisted `&view` address in a **high**
   register (`r8..r11`) so its remat is `add.w` (4 B), matching the shipped
   `ldr.w fp,[sp,#40]`. Every frame-neutral source form either folds to the
   committed codegen or grows the 208-B frame / shifts the whole body.
2. The `i*3 -> r4` flip is achievable (`mag` cache, radius-block move,
   `expect(isAster,0)`) but is neither necessary nor sufficient for the phase
   shift, and none of those forms is size-exact 1984.
3. The rotated k-loop is reachable (`for(;;)`/`do-while`) but is +4 B and puts
   the cold then-block out of line; `expect(diff>0,0)` remains the unique
   1984-B size anchor.

The committed **455 / 93 / 1984** body remains the best measured shape.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13471
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
# -> raw=455 aligned=93 size=1984/1984 insn=660/660
```

## 5. Files changed

`_work/reconstructions/gun-calccollision-wave190-w13471.md` only.
