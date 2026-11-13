# `Gun::calcCharacterCollision` wave 191 (w13553): ~100 more size-exact
# reshapes, all ≥ 455; the residual is a pure greedy-RA region-split plateau

Worker: w13553. Branch `wave191b-w13553`, base master `817cf1059`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13553`.

## 0. Result

**No `src/` change landed.** The committed body remains the best measured
shape: **raw 455 / aligned 93 / 1984 B**, 660/660 insns.

| build | raw | aligned | size |
|---|---:|---:|---:|
| before (HEAD `817cf1059`) | **455** | **93** | 1984/1984 |
| after this pass (note only) | **455** | **93** | 1984/1984 |

`git status --porcelain` is empty apart from this note.

## 1. Exact byte-diff decomposition (re-confirmed)

The 455 residual bytes are exactly four clusters, nothing else:

| region | bytes | cause |
|---|---:|---|
| `0x15306f..0x15315a` | 10 | `i*3` in r5 (ours) vs r4 (orig); 6 `add.w`/`mov` sites |
| `0x15316e..0x153194` | 6 | branch offsets caused by the 226-B phase shift below |
| `0x153198..0x15327b` | 226 | phase shift: ours emits 2-B `add r4,sp,#104` at `0x153198`/`0x153260` where orig has 4-B `ldr.w fp,[sp,#40]` (reload of `i`), and ours has one extra 4-B `ldr.w fp,[sp,#40]` at `0x153278` |
| `0x1533c8..0x1533f5` | 44 | view block: ours remats `&view` into r0/r1/r4 (3× 2-B `add`), orig keeps it in r5 across the block (1× `add` + 3× 2-B `mov`) |
| `0x153424`,`0x153438` | 2 | `i*3` register |
| `0x153440..0x153507` | 167 | k-loop block placement (see §3) |
| `0x15360a`,`0x15361e`,`0x15363a` | 3 | `i*3` register |

Everything else in the 1984-byte body is byte-identical.

## 2. New evidence: the view-block source shape is right; only RA differs

The original's view block is *not* unique to this function. `AEGeometry::translate`
(`0xa4992`) compiles the identical pattern

```c++
Matrix m = *(Matrix *) Canvas->TransformGetLocal(...);   // inlined memcpy
AEMath::MatrixSetTranslation(m, x, y, z);
Canvas->TransformSetLocal(..., m);
```

and there the original also keeps `&m` in **one** callee-saved register (`r9`)
across the whole block (`add.w r9, sp, #64` once, then `mov r1,r9` / `mov r0,r9`
/ `mov r1,r9` / `mov r2,r9`). So the committed `Matrix view = *(Matrix *)…`
source form is correct; the 44-byte view-block delta is purely greedy RA
choosing to rematerialise `%stack.10` per use in our build.

## 3. The k-loop is *not* rotated — it is the same CFG with different block placement

Both bodies have the same CFG (header = condition block, single latch). The
difference is only `MachineBlockPlacement`:

* orig: `body … then-block (inline) … latch … header … bcc body` (header at the
  bottom, then-block falls through);
* ours: `header … body … latch … then-block (out of line) … b.n latch`
  (header at the top, then-block cold).

The layout is driven entirely by the `__builtin_expect(diff > 0, 0)` branch
weight: with `expect(…,0)` the then-block is 0.05 % and goes out of line; with
`expect(…,1)` or no expect it is hot, the layout matches the original — but the
whole function then re-allocates (r4=this instead of r9=this) and shrinks to
**1908 B / 646 insns**, raw 1887. So the committed `expect(…,0)` is a *size
anchor*: it is what keeps 1984 B / 660 insns, at the cost of the layout.

## 4. Cells measured this pass (all `link_so` + `tools/fn_diff.py`, `-j2`)

Size-exact 1984 unless stated; none beats 455.

### 4.1 k-loop form

| variant | raw | aligned | size |
|---|---:|---:|---:|
| committed E-C-D, `expect(diff>0,0)` | **455** | **93** | 1984 |
| faithful C-D-E order, `expect(diff>0,0)` | 458 | 63 | 1984 |
| `expect(magnitude>distance,0)` + `diff` inside, E-C-D | 460 | 89 | 1984 |
| **`expect(magnitude>distance,0)` + `diff` inside, C-D-E** | 463 | **57** | 1984 |
| `diff >= 1` / `0 < diff` / `k < count` / `k < (unsigned)count` | 455 | 93 | 1984 |
| `if (diff)` (nonzero) | 1047 | 135 | 1980 |
| `if (diff > 0)` / `expect(diff>0,1)` / `expect(diff>0,2)` | 1887 | 281 | 1908 |
| `if (diff <= 0) continue;` | 1887 | 281 | 1908 |
| `expect(diff <= 0, 0)` | 455 | 93 | 1984 |
| `expect` on the loop condition (likely) | 1798 | 379 | 1948 |
| clamp as `std::min`/`std::max`, ternary, reversed, `>=`/`<=` | 1012–1747 | — | 1968–2000 |
| `hitFlags[i]`/`hitPositions[i]` moved after the k-loop | 1801–1859 | — | 1952–1964 |

Note the `mag`+C-D-E form is the *faithful* source (orig emits
`cmp magnitude,distance; ble` before `subs diff`, then clamp → damage → damageEmp)
and its k-loop body is instruction-for-instruction identical to the original
(aligned drops 93 → 57); it loses on raw only because the 226-B phase shift and
the loop placement are unchanged. It is not a raw win, so it was not landed.

### 4.2 `expect` sweep on the no-expect base (24 branch sites)

Every branch condition in the function was tried with `expect(cond, 0)` on top
of the hot-then-block base. None reaches 1984 B: best is
`expect(geometries != nullptr, 0)` at 1968 B (raw 1310); the rest are
1904–1928 B. Only `expect(diff > 0, 0)` produces the 1984-B layout.

### 4.3 view block / scatter block / declarations

Codegen-identical (455/93/1984): `Matrix &view = *(Matrix *)…` is *not*
(1195/1972); `const Matrix &cam = …; Matrix view = cam;` (455); `const Matrix
*cam = …; Matrix view = *cam;` (455); `Matrix view` before the two
`SetAnimationState` calls (1162/1988); between them (502/1984); `SetAnimationState
1` before `3` (457/1984); `Transform *t = …` local (1174/1968);
`TransformSetLocal(…, MatrixSetTranslation(view,…))` one-liner (1324/1976);
`const Vector &playerPosition` (455); `PlayerEgo *ego = getPlayer()` (455);
`radius` as if/else (455); `const int radius` (build error: `radius *= 5`);
`radius` ternary inverted (455); `radius*3.0f` (463); `radius*2.0f` (1756/1980);
`bool`/`int`/`const bool` for `mine`/`scatter`/`explosive`/`explodeOnContact`
(all 455); `bomb||mine` → `mine||bomb` (455); `isRocketLike||bomb` (1863/1916);
`enemy->isAsteroid()` cached in a local (455); `enemies->data()[e]` (455);
`positions[i]`/`positions.data()+i`/`Vector position(...)` (455); `td` as a
reference (455); `hitPositions[i]` (455); `diff` declared at k-loop top (455);
`int`/`unsigned` `distance` (455/94); `falloff` renamed `f` (455);
`(float)diff / magnitude` (455); `count = ++field + …` rewrite (455);
`nested impact` inner check removed (1096/1980); `outer` computed before the
egoFlag check (1789/1976); `ki` hoisted before `radius` (1771/1952).

## 5. Root cause (unchanged from w13483, now fully explained)

`%stack.10` (the 60-byte slot shared by `view`, the scatter `playerPosition`
and the k-loop `getPosition` sret) is hoisted by `Early MachineLICM` to the
e-loop preheader. `RAGreedy::tryRegionSplit` then splits it at the
`isAsteroid` branch (bb.43 / bb.55) and rematerialises `$r4 = t2ADDri %stack.10`
at both successors, so r4 is occupied across the whole i-loop body and `i*3`
falls to r5. The original rematerialises the same value per use (short ranges),
leaving r4 for `i*3` and r5 for `&view`.

All eight callee-saved registers are live (r4 `&stack.10`, r5 `i*3`, r6
`td`/`i*3` save, r7 frame pointer, r8 `enemy`, r9 `this`, r10 `&position`,
r11 `i`/`radius`), so the swap cannot be forced by freeing a register — only by
changing the region-split decision, and every source form tried that does so
also moves the size off 1984.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13553
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
# -> raw=455 aligned=93 size=1984/1984 insn=660/660
```

## 7. Files changed

`_work/reconstructions/gun-calccollision-wave191-w13553.md` only.
