# `Gun::calcCharacterCollision` + `SpaceLounge::drawLounge` wave 189 (w13327):
# both residuals remain RA/remat plateaus; no size-exact raw win

Worker: w13327. Branch `wave189-w13327`, base master `cbcdc0bc4`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch: `/home/debian/GalaxyOnFire2/scratch/w13327`.

## 0. Result

**No `src/` change landed.**  Every size-exact shape measured this wave is
equal to or worse than the committed body.  The tree is clean apart from
this note.

| symbol | raw | aligned | size | pct |
|---|---:|---:|---:|---:|
| `_ZN3Gun22calcCharacterCollisionEv` | **458** | 63 | 1984/1984 | 93.4 |
| `_ZN11SpaceLounge10drawLoungeEv` | **175** | 41 | 1948/1948 | 95.7 |

Gate on entry and exit (full `check`, `-j2`, `ulimit -v 12000000`):
**RATCHET PASS**, `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size-exact 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, data 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370, `.rel.plt` byte-identical, 27/27 headers, file size 2,187,328
exact.

This note confirms the wave-185..189 conclusions with two new pieces of
evidence: (a) the Gun `r4`/`r5` swap is caused by a **rematerialised stack
address live across the loop's `continue` edge**, not by the `i*3` vreg
itself; (b) the SpaceLounge `pad`/`x` colouring is invariant under every
source ordering of the four declarations (24 permutations measured).

## 1. `Gun::calcCharacterCollision` — 458/63/1984

### New evidence: the `r4`/`r5` swap is a remat of the shared `view` slot

The shipped and ours are identical up to the `i`-loop body.  At the
`isAsteroid` join the two diverge by one instruction:

```
shipped 153196  ldr  r0,[sp,#36]        ; mine
        153198  ldr.w fp,[sp,#40]       ; reload i (4 B)
        15319c  ldr  r6,[sp,#24]        ; td
ours    153196  ldr  r0,[sp,#36]
        153198  add  r4,sp,#104         ; &stack.10 (2 B)
        15319a  ldr  r6,[sp,#24]
```

The 2-byte substitution shifts the whole collision-check block by two
bytes (the 158-byte and 42-byte runs).  The final (post-VirtRegRewriter)
MIR shows that `sp+104` is the shared `Matrix view` / scatter-temp slot
(`fi#10`, 60 B) and that the address is **rematerialised**, not kept:

```
# after VirtRegRewriter, mode==0 branch
$r11 = t2LDRi12 $r6, 148      ; pad   (shipped: r9)
$r10 = t2LSLri  $r11, 1       ; pad*2 (shipped: sl)
...
$r9  = COPY $s18              ; x     (shipped: r5)
```

Because the remat is live across the loop's `continue` edge
(`15325c b.n 15305e`), it occupies `r4` for the whole loop body, so the
scatter block's `playerPosition` sret address is forced into `r5`:

```
shipped 1530d2  add r4,sp,#44   ; &playerPosition
        1530dc  add r5,sp,#104  ; &delta
ours    1530d2  add r5,sp,#44
        1530dc  add r4,sp,#104
```

The shipped does not hoist the address; it materialises `&view` locally
in the view block and keeps it in `r5` across the memcpy:

```
shipped 1533c8  add r5,sp,#104 ; &view
        1533ce  mov r0,r5
        1533de  mov r1,r5
        1533ec  mov r2,r5
ours    1533ca  add r0,sp,#104
        1533dc  add r1,sp,#104
        1533ea  add r4,sp,#104
        1533f4  mov r2,r4
```

So the residual is a single Early-Machine-LICM/remat decision on the
stack address; every source shape that keeps the committed tail keeps it.

### Measurements this wave (full link + `tools/fn_diff.py`, `-j2`)

All variants keep the function at 1984 B unless stated.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| committed | **458** | 63 | 1984 |
| named `Vector delta = enemyPosition - playerPosition;` | 458 | 63 | 1984 |
| `Matrix view; Matrix *vp=&view; *vp=...; ...*vp` | 1888 | 248 | 2056 |
| `__builtin_memcpy(&view, ..., sizeof(Matrix))` | 1923 | 249 | 2064 |
| `Matrix view; view = *(...)` | 1888 | 248 | 2056 |
| `view = MatrixSetTranslation(view, ...)` | 1097 | 107 | 1992 |
| `Vector delta; delta = enemyPosition - getPosition();` | 1645 | 288 | 1964 |
| declaration order `PVE` (committed) | **458** | 63 | 1984 |
| `PEV` / `VPE` / `VEP` / `EPV` / `EVP` | 503/473/512/541/541 | 87/86/102/107/110 | 1984 |
| `expect(0 < diff, 0)` | 458 | 63 | 1984 |
| `expect(diff >= 1, 0)` | 458 | 63 | 1984 |
| `expect((bool)diff, 0)` | 1042 | 105 | 1980 |
| `expect(diff != 0, 0)` | 1042 | 105 | 1980 |
| `expect(diff > 0, 1)` | 1888 | 263 | 1908 |
| `Matrix view` declared at i-loop scope / before `if (impact)` | 458 | 63 | 1984 |

The `expr` pp form (`VectorLength(enemyPosition - getPlayer()->getPosition())`)
is 462/73/1984: it reproduces the shipped stack slots (`playerPosition`
sp+44, `delta`/`view` sp+104) but leaves the same `r4`/`r5` swap.

## 2. `SpaceLounge::drawLounge` — 175/41/1948

### New evidence: the `pad`/`x` colouring is invariant under source order

The final MIR (`-mllvm -print-after-all`, post-VirtRegRewriter) confirms
the committed colouring:

```
$r11 = t2LDRi12 $r6, 148   ; pad      (shipped: r9)
$r10 = t2LSLri  $r11, 1    ; pad*2    (shipped: sl)
$r9  = COPY $s18           ; x        (shipped: r5)
```

At the pad load the free callee-saved registers are `r9,r10,r11`; the
allocator picks `r11`.  Every source reordering of the four statements
`pad`, `SetColor`, `centerX`, `top` (24 permutations) yields either the
committed 175 (pad load scheduled first) or 254 (pad load scheduled after
`SetColor`); none moves `pad` to `r9`.

### Measurements this wave (full link + `tools/fn_diff.py`, `-j2`)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| committed | **175** | 41 | 1948 |
| `pad` after `centerX` / after `SetColor` | 254 | 72 | 1948 |
| `const int pad` / `register int pad` | 175 | 41 | 1948 |
| `volatile int pad` | 1864 | 384 | 1980 |
| `centerX` first | 175 | 41 | 1948 |
| single `int x` (rename `centerX`→`x`) | 175 | 41 | 1948 |
| `pad` after the strings | 1478 | 278 | 1952 |
| RHS `- (width + (pad + pad))` | 445 | 54 | 1948 |
| RHS `- width - pad - pad` | 468 | 64 | 1944 |
| RHS `- (width + pad + pad)` | 176 | 42 | 1948 |
| RHS `+ (-(width + pad + pad))` | 176 | 42 | 1948 |
| left clamp `x < inset + pad` | 492 | 73 | 1944 |
| left clamp `pad > x - inset` | 178 | 43 | 1948 |
| right cond `x + pad + width` / `width + pad + x` / `x + width + pad` | 175 | 41 | 1948 |
| `lineHeight` direct / `Globals::layout->...` / `const` | 175 | 41 | 1948 |

The committed `+ (-width - pad - pad)` association remains the unique
175.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13327
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv _ZN11SpaceLounge10drawLoungeEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=458/175`, `aligned=63/41`, sizes `1984/1948`,
`RATCHET PASS`, byte 4507 / linked 4507, data 843/843, imports 207,
lint CLEAN, drift 0.

Machine-IR evidence (scratch, not tracked):

```sh
# Gun: post-VirtRegRewriter MIR shows the sp+104 remat and the pad/x
# colouring; scratch/w13327/gun.ll has the view block (lifetime.start 60
# for %64 = view and %65 = MatrixSetTranslation sret temp).
# SpaceLounge: post-VirtRegRewriter MIR shows $r11 = pad, $r10 = pad*2,
# $r9 = x.
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13327/`
(`Gun.cpp.base2`, `SL.cpp.base2`, `gsw.sh`, `ssw.sh`, `gun_perm.py`,
`gun_exp.py`, `gun_view.py`, `sl_perm.py`, `sl_clamp.py`, `sl_lh.py`,
`sl_final.mir`, `gun.ll`).

## 4. Files changed

`_work/reconstructions/gun-spacelounge-wave189-w13327.md` only.

## Harvest on wave-189 master (`cd3f94ba2`)

Note-only: Gun stays 458 / 63 / 1,984, SpaceLounge 175 / 41 / 1,948.
Current master is `.text` 73,981 and total 74,616.  The Gun `r4`/`r5` swap is
a single Early-Machine-LICM/remat decision around the `Matrix view` lifetime;
the SpaceLounge `pad`/`x` allocation is a post-`VirtRegRewriter` plateau.
