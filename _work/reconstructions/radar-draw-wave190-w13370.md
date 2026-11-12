# Radar::draw wave 190 (w13370) — spark `inSight` second bound rewritten as a
# subtraction: raw 6,308 -> 6,307 (size-exact, `.rel.plt` identical)

**Status:** landed, 2026-11-12. Worktree `wt/w13370`, branch `wave190-w13370`, base `master`
`0f54626b3`. Target `src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave189-w13356.md` (landmark ternary, raw 6,309 -> 6,308),
`radar-draw-wave189-w13346.md` (wreck x/y / music-tail analysis) and
`radar-draw-wave189-w13331.md` (campaign-in-`sl` family). This pass swept the whole draw body
with an operand-order comparison harness (224 `L op R` -> `R mirror(op) L` cells) and then
worked the one hit.

## 0. The win

One source edit in the turret-spark `inSight` block (line 968):

```cpp
-                        inSight = this->screenX > left && this->screenX < left + size * 2 && this->screenY > (int) top &&
+                        inSight = this->screenX > left && left > this->screenX - size * 2 && this->screenY > (int) top &&
                                   this->screenY < (int) top + size * 2;
```

The two forms are algebraically identical (`screenX < left + 2*size` <=> `screenX - 2*size < left`).

| metric | base | after |
|---|---:|---:|
| raw same-address diff | **6,308** | **6,307** |
| instruction-index alignment | 1,858 | 1,862 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,541 / 3,532 | 3,541 / 3,532 |
| `.text` differing bytes (whole `.so`) | 73,516 | **73,515** |
| `.ARM.extab` differing bytes | 610 | 610 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| build-id differing bytes | 20 | 19 |
| total differing bytes | 74,146 | **74,145** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 / imports 207 /
lint CLEAN / parity 5,370. The function was already non-byte-exact, so `byte_exact` is
unchanged; the gain is in the raw residual only.

## 1. Why it helps

The original spark block (`12f4d6..12f4e4`) is

```
12f4d6: vmov  r2, s2                  ; r2 = (int) left
12f4da: cmp   r1, r2                  ; screenX vs left
12f4dc: ble.n skip                    ; screenX <= left -> skip
12f4de: add.w r2, r2, r0, lsl #1      ; r2 = left + size*2   (eb02 0240)
12f4e2: cmp   r1, r2
12f4e4: bge.n skip                    ; screenX >= left+size*2 -> skip
```

The base emits `add.w r3, r3, r0, lsl #1` (`eb03 0340`) because clang keeps `left` in `r3`
and `screenX` in `r1`; that differs from the original in 2 bytes. Written as the subtraction
`left > screenX - size*2`, clang keeps `left` in `r3` but materialises the bound with
`sub.w r2, r2, r0, lsl #1` (`eba2 0240`) where `r2` is `screenX`; that encoding differs from
the original's `eb02 0240` in only one byte, so the same-address raw count drops by one.
Instruction count, size, section layout, `.ARM.extab` and `.rel.plt` are unchanged.

## 2. Sweep that found it

`scratch/w13370/cmp_sweep2.py` extracts every top-level `L op R` comparison in the draw body
(224 cells), rewrites it to `R mirror(op) L` and rebuilds. Only one cell was a size-exact
improvement; the rest were either byte-identical (`raw=6308`) or size-fatal. The harness
leaves `src/` at the last variant, so restore the base with
`cp scratch/w13370/Radar.base.cpp src/game/weapons/Radar.cpp`.

Equivalent spellings measured on the same site (all size 9,908):

| spelling | raw |
|---|---:|
| base `screenX < left + size * 2` | 6,308 |
| **`left > screenX - size * 2`** | **6,307** |
| `screenX - size * 2 < left` | 6,307 |
| `left > screenX - (size << 1)` | 6,307 |
| `screenX - left < size * 2` | 6,311 |
| `(int)(screenX - left) < size * 2` | 6,311 |
| `size * 2 > screenX - left` | 6,311 |
| `left - size * 2 < screenX` | 6,308 |
| `screenX < size * 2 + left` | 6,308 |
| `left + size * 2 > screenX` | 8,273 (size 9,904) |
| `left > screenX + size * 2` (not equivalent) | 6,306 |

`left > screenX + size * 2` is 6,306 but is *not* algebraically equivalent to the base, so the
equivalent `left > screenX - size * 2` (6,307) is the one landed.

## 3. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6307 aligned= 1862 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13370/`: `Radar.base.cpp` (6,308 base),
`cmp_sweep2.py` + `cmp2_list.txt` + `cmp2.out` (224-cell comparison sweep), `try3.py` /
`try4.py` (equivalent-spelling sweep), `base.so` / `v2.so`, `secs.py` (section byte diff),
`cmpbytes.py` (per-run byte diff between two `.so`s).

## Current-master rebase (wave 190 harvest)

Re-applied on current master (`454bc033e`) after the w13371 station-blip/tractor2
landing. Current master before this change was raw 6,299; after it is raw 6,298
(-1), aligned 1,798 -> 1,802, size 9,908/9,908. `.text` 73,302 -> 73,289 (-13
across LevelScript + Radar + Gun), `.ARM.extab` 603 unchanged, total 73,925 ->
73,912. `check` green, RATCHET PASS.
