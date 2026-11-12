# `Gun::calcCharacterCollision` wave 190 (w13406): hoist-prevention flips the
# `i*3`/`&view` r4/r5 swap but not size-exactly — no landed win

Worker: w13406. Branch `wave190-w13406`, base master `f0982704f`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13406`.

## 0. Result

**No `src/` change landed.**  The committed body (wave-190 w13388) remains the
best measured shape: **raw 455 / aligned 93 / 1984 B**, 660/660 insns.
Tree clean apart from this note.

| build | raw | aligned | size |
|---|---:|---:|---:|
| before (HEAD `f0982704f`) | **455** | **93** | 1984/1984 |
| after this pass (note only) | **455** | **93** | 1984/1984 |

Gate on entry/exit: `check` green (byte_exact 4507/4524, linked_exact
4507/4524, size-exact 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370).

## 1. Where the 455 bytes actually are (re-measured)

`tools/fn_diff.py` raw groups (same-address byte diff):

| region | bytes | cause |
|---|---:|---|
| `0x15306f`..`0x15315a` | 7 | `i*3` in r5 (ours) vs r4 (orig) |
| `0x15316e`..`0x153194` | 8 | branch offsets from the 2-byte phase shift |
| `0x153198`..`0x15327c` | **226** | 2-byte phase shift: ours remats `&view` into r4 at `0x153198`; orig reloads `i` into fp (4-byte `ldr.w`) |
| `0x1533c8`..`0x1533f6` | **44** | view block: ours remats `sp+104` into r0/r1/r4; orig keeps it in r5 |
| `0x153424`..`0x153508` | **169** | k-loop: ours top-tests (body out of line); orig rotates with body fallthrough; also the `diff`/clamp/EMP order |
| `0x15360a`..`0x15363a` | 3 | `i*3` in r5 vs r4 (mine block) |

So **every** residual byte is the single greedy-RA decision `i*3 -> r5,
hoisted-ADDri(&view) -> r4` (the wave-189 w13353/w13365/w13388 plateau), plus
the k-loop block placement that is a consequence of the `expect(diff>0,0)`
size anchor.

## 2. New angle this pass: hoist-prevention flips the swap

The wave-189 note proved the hoisted `t2ADDri %stack.10` (address of the
`Matrix view` alloca) is *always* hoisted out of the e-loop preheader because
it is trivially rematerializable, so its live range is ~16 B longer than
`i*3`'s and greedy RA (size-descending for globals) gives it r4 first.

New experiment: make the `view` address **depend on the e-loop induction
variable** so MachineLICM can only hoist it to the e-loop *body* (i-loop
preheader), shortening its range below `i*3`'s:

```c++
Matrix views[2];
Matrix &view = views[e & 1];
view = *(Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent());
AEMath::MatrixSetTranslation(view, position.x, position.y, position.z);
Globals::Canvas->TransformSetLocal(this->geometries[i], view);
```

This **does** flip the swap: `i*3` is now `add.w r4, r6, r6, lsl #1`
(`0x15307a`) and the view block remats into r4.  But it is not usable:

| variant | raw | aligned | size | frame |
|---|---:|---:|---:|---:|
| committed | **455** | **93** | 1984 | 208 |
| `views[2]; view = views[e & 1]` | 1664 | 341 | **1984** | 268 |
| `views[2]; view = views[(e>>31)&1]` | 1665 | 342 | 1984 | 268 |
| `views[1]; view = views[i*0]` (folded) | 1138 | 201 | 1988 | 208 |
| `Matrix &ref = *(Matrix*)((char*)&view + (i>>31))` | 1097 | 141 | 1992 | 208 |
| `Matrix &ref = *(Matrix*)((char*)&view + zero)` (`volatile zero`) | 1418 | 180 | 1992 | 208 |
| `Matrix &ref = *(Matrix*)((char*)&view + (e>>31))` | 1906 | 289 | 2072 | 208 |

The extra 60-byte Matrix slot grows the frame 208 -> 268, which shifts every
sp-relative load/store and dominates the raw metric (1664).  The frame-neutral
`&view + non-foldable-0` forms add 8 B (a shift + an extra address add) and
also shift the whole body (raw ~1100).  `(e>>31)`, `((e&1)>>1)`, `(e&1)*0`,
`(e>>31)>>1` are all constant-folded by clang 7's range analysis (455,
codegen-identical).

## 3. Other cells measured this pass (all `link_so` + `tools/fn_diff.py`, `-j2`)

All size-exact unless stated; none beats 455.

- **Statement order** (E=EMP block, C=clamp, D=damage): ECD **455**,
  CED 457, CDE 458, DCE 463, EDC 1794/1940, DEC 1803/1940.  Clamp branch
  order reversed (E+Cn+D) 456.  `emp` block last (C+D+E) 458.
  Duplicate-division EMP 2000.
- **`expect` placement**: clamp 455; impact 455; i-loop / e-loop condition
  455 (identical); k-loop condition 1798/1948; `empDamage` 927/928;
  scatter 1613/1828; `geometries` 1608/1378; asteroid 1796/1813;
  `explodeOnContact` 1635/665; `empDamage != kNoAttribute` 457/1351;
  owner 1880/1891; `playerGun` 1887/1867; `explosive` 1831/1860;
  `outer` 1711/1641; `outerCollide` 1821/1720; `ki->field_0x71` 1417/1547;
  `distance > 20000` 1425/1629; egoFlag 1341/455.
  `__builtin_expect_with_probability(diff>0,1,p)` for p in
  {0.0,0.01,0.05,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0} is
  codegen-identical to the committed 455 (the probability is ignored; only
  the `expect` *presence* keeps size 1984).  Removing the `expect` entirely
  (or `expect(...,1)`) is always 1908/1864 — the `expect` is the size anchor.
- **Types**: `unsigned radius` 459; `unsigned distance` 458;
  `unsigned diff` 455/94; `const` on enemy/radius 2048; `const` on
  outer/diff/falloff/position/velocity/enemyPosition 455; `int i` 456,
  `int e` 456, `i != count` 456, `e != size` 456; `size_t`/`unsigned long`
  455.
- **i-loop prefix**: full 24-permutation sweep of
  (position, velocity, enemyPosition, radius) — PVER **455** best; next
  VPER 470; E-first 538+; R-first 1784+.
- **View block**: `Matrix*`/`Matrix&`/`const`/`volatile Matrix*`/
  `reinterpret_cast`/`memcpy`/`__builtin_memcpy`/`static`/`views[2]` — all
  either 455 (folded) or 1988..2096.
- **SetAnimationState / copy order**: 3,1,copy **455**; 1,3,copy 459;
  3,copy,1 512; 1,copy,3 513; copy,3,1 / copy,1,3 1988.
- **Radius ternary forms** (if/if2/tern2/tern3), **loop forms**
  (post/+=1/int/size_gt/ne), **merged checks** (`mine && ...`,
  `!isActive() || hp<1`), `td` removed / `this->targetDir` direct,
  `Level* lvl` cache, `fg`/`item`/`emp` locals, `MatrixSetTranslation`
  Vector overload — all codegen-identical (455).
- **Clamp forms**: two ternaries 1960; one ternary 1968; two `if`s 1960;
  `if_ge` 455; `if_neg` 456.
- **`field_54` byte store**: `enemy->field_54 = 1;` 456 (cast form is
  better); `*(uint8_t*)&enemy->field_54 + 1` 455.

## 4. Conclusion

The r4/r5 swap is a genuine greedy-RA plateau.  The only lever found that
flips it (make the `&view` ADDri depend on `e`) forces a +60 B frame and a
worse raw metric.  A frame-neutral hoist-prevention needs a non-foldable
zero offset, which costs >= 8 B and shifts the body.  The committed
**455 / 93 / 1984** body remains the best measured shape.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13406
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
# -> raw=455 aligned=93 size=1984/1984 insn=660/660
```

## 6. Files changed

`_work/reconstructions/gun-calccollision-wave190-w13406.md` only.
