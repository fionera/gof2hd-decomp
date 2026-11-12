# `Gun::calcCharacterCollision` + `SpaceLounge::drawLounge` wave 188 (w13220):
# both residuals remain RA/hoist plateaus; no size-exact raw win

Worker: w13220. Branch `wave188-w13220`, base master `ce16f45f1`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch: `/home/debian/GalaxyOnFire2/scratch/w13220`.

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

This note records the new angles tried this wave and confirms the wave-185
to wave-187 conclusions.  It is self-contained; the committed body is
unchanged.

## 1. `Gun::calcCharacterCollision` — 458/63/1984

### Residual (unchanged, re-measured)

The 49 differing byte runs collapse to the same eight clusters:

1. `i*3` register `r5` vs shipped `r4` (8 sites: `0xdf 0xea 0xfe 0x1ca
   0x4a6 0x678 0x68c 0x6a8`).
2. `playerPosition` slot `sp+104` vs shipped `sp+44` (`0x142`).
3. `delta` slot `sp+44` vs shipped `sp+104` (`0x14c`).
4. `mov r6,r5` vs shipped `mov r6,r4` (`0x13c`).
5. post-`isAsteroid` `add r4,sp,#104` vs shipped `ldr.w fp,[sp,#40]`
   (`0x208`, `0x2d0`).
6. `kNoAttribute` load `ldr r0,[pc,#136]` + extra `ldr.w fp,[sp,#40]`
   vs shipped `ldr r0,[pc,#132]` (`0x2e6`).
7. `view` block rematerialisation (`0x438`-`0x466`).
8. rotated k-loop vs top-test k-loop (`0x4b0`-`0x578`).

The dominant cluster is still (5): the shipped instruction at `0x208` is a
4-byte reload of `i`; ours is a 2-byte rematerialisation of `&stack.10`.
That 2-byte substitution shifts the whole collision-check block
(`0x20a`-`0x2ec`) by two bytes, which is why the `0x223`-`0x2c1` run is
158 bytes and the `0x2c2`-`0x2ec` run is 42 bytes.  A hypothetical +2-byte
insertion before `0x223` would align 159/159 bytes of `0x223`-`0x2c2`
(measured by shifting ours +2) and 34/48 bytes of `0x438`-`0x468`.

### New sweeps this wave (full link + `tools/fn_diff.py`, `-j2`)

All variants keep the function at 1984 B unless stated.

| variant | raw | aligned |
|---|---:|---:|
| committed | **458** | 63 |
| `expr` pp form (wave-123) | 462 | 73 |
| `expr` + named `Vector delta` in k-loop | 1880 | 456 (size 1876) |
| `expr` + `Vector delta = getPosition() - position` | 1789 | 279 (size 1952) |
| `stmt` + named `Vector delta` in k-loop | 1880 | 461 (size 1876) |
| `stmt` + `Vector delta = getPosition() - position` | 1789 | 288 (size 1952) |
| `Vector delta = enemyPosition - getPosition()` (expr pp) | 462 | 73 |
| `Vector delta(enemyPosition - getPosition())` | 462 | 73 |
| `pp` named + `delta` named | 458 | 63 |
| `const Vector &pp` | 458 | 63 |
| `Matrix view` copy-init / reinterpret / `Matrix(...)` | 458 | 63 |
| `Matrix view; view = ...` | 1888 | 248 (size 2056) |
| `Matrix &view = ...` (no copy) | 1200 | 147 (size 1972) |
| `alignas(4/8) Matrix view` | 458 | 63 |
| `alignas(16/32) Matrix view` | 1861/1863 | 215 (size 1996) |
| `Matrix view` at function scope | 1884 | 408 (size 1960) |
| `#pragma clang loop unroll/vectorize/interleave/distribute` on i/k/both | 458 | 63 |
| cache `i`, `hitFlags`, `hitPositions` before scatter | 458 | 63 |
| cache `positions.data()` / `velocities.data()` | 480 | 67 |
| `e` / `i` loop index `int`/`size_t`/`uint32_t` | 458 | 63 |
| `while(true)` / `for(;;)` / `for(; ; ++k)` / increment-in-body k-loop | 458 | 63 |
| `do { } while (++k < n)` k-loop | 1032 | 156 (size 1988) |
| `if (expect(diff <= 0, 1)) continue;` | 950 | 109 (size 1972) |
| `if (!expect(diff > 0, 0)) continue;` | 458 | 63 |
| `if (expect(diff > 0, 0)) { } else { }` | 458 | 63 |
| swap the two `SetAnimationState` calls | 460 | 65 |
| swap animation calls + all 120 tail orders | best 460 | 65 |
| `expect(empDamage != kNoAttribute, 1)` + all 120 tail orders | best 460 | 66 |
| `expect(playerGun, 1)` / `expect(impact != nullptr, 1)` | 458 | 63 |
| `expect(scatter, 1)` / `expect(mine, 0)` | 1802/1686 | 331/218 |
| `expect(diff >= 1, 0)` / `expect(diff != 0, 0)` | 458 | 63 |
| move the explosive/geometries block before field54/pgun/setHit/lifetimes/lastHit/emp/damage | 1252/1379/1260/1242/1138/1531/1301 | 104-268 |
| move `lifetimes[i] = -1000000` before emp/damage/asteroid/impact | 1325/1327/1388/696 | 82-124 |
| move scatter block before impact | 1788 | 217 (size 1972) |
| move impact block before tail | 1381 | 99 (size 1980) |

The `expr` + named-delta family changes the frame size, so it cannot be
used.  The tail-permutation sweeps on the `swap_anim` and `emp1` bases
confirm that no `ABCDE`-tail shape reaches below 460 on those bases; the
committed body is the unique 458.

### Conclusion

The residual is still the Early Machine LICM hoist of `&stack.10` and the
greedy-RA colouring it forces.  Every source shape that keeps the shipped
`ABCDE` tail keeps `i*3 = r5`; every shape that moves `i*3` to `r4`
(`expr` + `BCEAD`/`BECAD`) changes the emitted tail order and loses more
bytes than it saves.  The committed `stmt` body remains the best measured
shape at **458/63/1984**.

## 2. `SpaceLounge::drawLounge` — 175/41/1948

### Residual (unchanged, re-measured)

The right-edge clamp still drives the residual:

```
shipped:  pad = r9, x = r5
          vmov r5,s18 ; sub.w r1,r5,r9 ; ... ; add.w r1,r9,r5
          itt gt ; addgt.w r1,r6,sl ; subgt r5,r0,r1
ours:     pad = fp, x = r9
          vmov r9,s18 ; sub.w r1,r9,fp ; ... ; add.w r1,fp,r9
          itttt gt ; movgt r1,#0 ; subgt.w r1,r1,fp,lsl#1 ;
          subgt r1,r1,r6 ; addgt.w r9,r1,r0
          mov r5,fp
```

The two `mov r5,fp` copies exist because `pad` lives in `fp` and `fp` is
reused for `lineHeight` after the clamp.  The shipped has no copies because
`pad` is in `r9` and `x` in `r5`.

The MIR mechanism is confirmed: `x` (`%608`) is selected first because of
the physical `$r3 = COPY %608` hint at the `DrawString` call; `pad` (`%2`)
is unhinted and has a larger live range (Size 6384 vs 4220), so without the
hint `pad` would be selected first.

### New sweeps this wave (full link + `tools/fn_diff.py`, `-j2`)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| committed | **175** | 41 | 1948 |
| `int drawX = x` / `int &drawX` / `int *drawX` used at both DrawString calls | 175 | 41 | 1948 |
| `long`/`unsigned`/`double`/`long long drawX = x` used at calls | 175 | 41 | 1948 |
| `short`/`float drawX` | 1861/1880 | 182/186 | 1972/1984 |
| `volatile int drawX = x` | 1871 | 289 | 1972 |
| `(int)x` / `x + 0` at the calls | 175 | 41 | 1948 |
| `int x = (int)centerX` / `centerX + 0` | 175 | 41 | 1948 |
| `unsigned int x` | 176 | 42 | 1948 |
| `int boxX = x - pad` (used by drawBox/DrawRectangle) | 175 | 41 | 1948 |
| `int boxY = top - pad` | 265 | 50 | 1948 |
| `int boxW = pad * 2 + width` | 1853 | 258 | 1968 |
| `boxX/boxY/boxW` locals | 175 | 41 | 1948 |
| `const int &pad` / `int &pad` | 1853 | 195 | 1972 |
| `const volatile int &pad` | 1890 | 305 | 2004 |
| `const int pad` / `register int pad` | 175 | 41 | 1948 |
| `String("", false)` for the empty role | 175 | 41 | 1948 |
| `String()` / `String(nullptr)` for the empty role | 591/569 | 82/73 | 1940/1944 |
| `String("  ", true)` | 178 | 44 | 1948 |
| `String name` / `String role` copy-init | 175 | 41 | 1948 |
| `String name(...)` if/else | 1679 | 200 | 1992 |
| `String role` if/else | 1740 | 219 | 2028 |
| `name`/`role` declaration swap | 1479 | 320 | 1896 |
| clamp condition `inset > x-pad` / `inset >= x-pad` / `x-pad >= inset` | 177/197/196 | 43/47/46 | 1948 |
| clamp condition `w-inset < pad+x+width` | 176 | 42 | 1948 |
| clamp condition `width+pad+x` / `x+pad+width` / `pad+width+x` | 175 | 41 | 1948 |
| nested `if (x-pad >= inset) { if (right) ... } else ...` | 176 | 41 | 1948 |
| ternary clamp | 175 | 41 | 1948 |
| two independent `if`s | 455 | 68 | 1948 |
| `expect` on first/second/both clamp conditions (0/1) | 492-579 | 71-93 | 1952-1956 |
| 2-insn clamp `+ (-width - (pad + pad))` | 445 | 54 | 1948 |
| 2-insn clamp + `lineHeight` hoisted before the clamp | **265** | 47 | 1948 |
| 2-insn clamp + `lineHeight` hoisted + all 24 `{pad,SetColor,centerX,top}` orders | 265 | 47 | 1948 |
| 2-insn clamp + `lineHeight` hoisted + `const`/`register`/`unsigned` pad | 265/265/266 | 47/47/50 | 1948 |
| 2-insn clamp + `lineHeight` hoisted + `drawX` copy | 587-621 | 164-194 | 1952-1960 |

### New finding: the 2-insn clamp + lineHeight hoist moves `pad` off `fp`

The wave-185/186 notes state that `pad` is `fp` in every size-1948 variant.
This wave found one size-1948 shape where `pad` is **`r5`** (not `fp` and
not `r9`): the shipped 2-instruction clamp association plus moving the
`int lineHeight = layout->field_0x30_rowHeight;` load before the clamp.
The emitted clamp becomes the shipped `itt gt; addgt.w r1,r6,sl; subgt
r5,r0,r1`, and the two `mov r5,fp` copies disappear.  The raw score is
**265** (down from 445 for the same clamp without the hoist), but still
worse than the committed 175 because `pad` and `x` are now swapped the
other way (`pad = r5`, `x = r9`) and the downstream `x` copies remain.

All 24 declaration orders of `{pad, SetColor, centerX, top}` and four
`lineHeight` positions on this base give the same 265.  `const pad`,
`register pad`, `long x`, `register x` are identical; `unsigned pad` is
266.  The `drawX` hint-removal copies change the frame size on this base.

### Conclusion

The shipped needs `pad = r9`, `x = r5` simultaneously.  The committed body
gives `pad = fp`, `x = r9`; the 2-insn + lineHeight-hoist family gives
`pad = r5`, `x = r9`.  No size-1948 source shape found this wave reaches
`pad = r9`.  The committed body remains the best measured shape at
**175/41/1948**.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13220
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
# Gun: Early Machine LICM hoists t2ADDri %stack.10; after RA the address is
# rematerialised at 0x196/0x208 instead of the shipped ldr.w fp,[sp,#40].
clang++ <Gun.cpp flags> -mllvm -print-after-all \
  -mllvm -filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv > gun_mir.txt
# SpaceLounge: Greedy RA dump shows $r3 = COPY %609 (x hint) and pad %2
# unhinted; the 2-insn + lineHeight-hoist base gives $r5 = pad, $r9 = x.
clang++ <SpaceLounge.cpp flags> -mllvm -print-after-all \
  -mllvm -filter-print-funcs=_ZN11SpaceLounge10drawLoungeEv > sl_mir.txt
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13220/`
(`sweep_gun*.py`, `sweep_sl*.py`, `diff.py`, `bytes.py`, `shift.py`,
`gun_mir.txt`, `sl_mir.txt`, `ra.txt`, `sl_ra.txt`, `vrr.txt`).

## 4. Files changed

`_work/reconstructions/gun-spacelounge-wave188-w13220.md` only.
