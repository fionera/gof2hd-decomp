# Radar::draw wave 189 (w13346) — register-allocation pass: the music-block else
# placement is gated by the outer `expect(field_0x1bc>=1,1)`; wreck block is the
# first structural tail divergence. No size-exact raw win.

**Status:** analysis complete, 2026-11-12. **No size-exact raw win found**; `src/` is left at
the base. This pass commits only this note. Worktree `wt/w13346`, branch `wave189-w13346`,
base `master` `f258b29c0`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave189-w13331.md` (alienOrbit spill), `radar-draw-wave189-w13335.md`
(planet-loop latch placement), `radar-draw-wave188-w13210.md` (108-cell cross product) and
`radar-draw-wave189-w13319.md` (register/branch map). This note is self-contained: every
number below was measured in this worktree on the 6,309 base.

## 0. Baseline (verified, unchanged)

| metric | value |
|---|---:|
| function address | `0x12e050` (not `0x12e000`; the preceding `Radar::update` occupies `12e000`–`12e04f`) |
| function size | 9,908 / 9,908 |
| raw same-address diff | **6,309** |
| instruction-index alignment | 1,858 |
| instruction count | 3,541 ours / 3,532 orig |
| frame | `sub sp,#296` (`0x128`) |
| `campaign` | spilled `sp+0x90` |
| shared zero | `sl` (`12e07a mov.w sl,#0`) |
| planet-loop counter `i` | `sl` |
| planet-loop `stations` | spilled `sp+0x84` |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` with `src/` at the base. No code change is committed by this pass.

## 1. Correct per-region map (the earlier notes' `12e000` bound is off by 0x50)

`tools/fn_diff.py`'s raw is a same-address byte count over the function, which starts at
`0x12e050`. Per-region same-address byte diffs of the base (scratch `regdiff2.py`, which
uses the real symbol address — the older `regdiff.py`/`heat2.py` index below the function
start and are wrong for `12e000`-based bounds):

```
12e050-12e3a4   diff=   22 /  852
12e3a4-12e3c6   diff=    5 /   34
12e3c6-12e7aa   diff=  953 /  996   planet loop
12e7aa-12ec24   diff= 1075 / 1146   crosshair body
12ec24-12ef6d   diff=  780 /  841
12ef6d-12f7cc   diff= 1610 / 2143   enemy loop
12f7cc-1306b4   diff= 1804 / 3816   asteroid / music / tail
```

The base is byte-identical to the original from `12f7cc` to `12fc4c` except for **19
single-byte branch-offset bytes** (all of them branch targets into the shifted tail). The
first *structural* divergence in the tail is the wreck x/y block at `12fc4c`.

## 2. The music-block lever is real but is gated by the outer expect

`radar-draw-wave189-w13335.md`'s suggested lever is the `field_0x1bc < 1` else block:
the original has `blt.n 1302f0` (2 B) with the else block **inline** right after the then
body, while the base has `blt.w 1304e2` (4 B) with the else block placed far out, after the
whole music block. This pass pinned down *why*.

* The original's then-body evaluation order is `range>15`, then `mask==0`, then
  `(current != 151 && alwaysEnemySeen)` (short-circuit `cmp/beq/ldr/eors/bne`).
* The base evaluates `(current != 151 && alwaysEnemySeen)` before the mask test and
  materialises `current != 151` as a 0/1 value (`subs/it/movne/ands`), which is 2
  instructions longer.
* The *else-block placement* is controlled entirely by the outer
  `__builtin_expect(this->field_0x1bc >= 1, 1)`: with weight 1 the else is cold and moved
  out of line (the base). Removing the expect puts the else inline at `1302e2`
  (`blt.n`), matching the original's layout — but it also re-colours the whole enemy loop
  (`12fb0c`–`13008c` becomes ~700 B different) and shifts the music block by `-4`.

Measured cells (all full `link_so -j2` + `fn_diff`):

| body | raw | aligned | size |
|---|---:|---:|---:|
| base | **6,309** | 1,858 | 9,908 |
| outer `expect(...,1)` -> plain `if` (`e1bc_plain`) | 6,486 | 1,780 | **9,916** |
| outer `expect(...,0)` (`e1bc_0`) | 8,519 | 1,823 | **9,904** |
| outer `expect(>0,1)` (`e1bc_gt0`) | 6,309 | 1,858 | 9,908 (byte-identical) |
| plain outer + inner reorder (range, mask, `&&`) (`ep_reorder`) | 6,452 | 1,769 | **9,912** |
| plain outer + inner reorder + `expect(&&,0)` (`ep_reorder_exp`) | 7,287 | 1,760 | 9,908 |
| inner reorder + `expect(&&,0)`, keep outer (`m_reorder_exp`) | 6,332 | 1,856 | 9,908 |
| `needStop = range>15 || mask==0; if (needStop || expect(&&,0))` (`m_n1`) | 6,332 | 1,856 | 9,908 |
| `expect(alwaysEnemySeen,0)` inside the `&&` (`m_n4`) | 6,313 | 1,859 | 9,908 |
| `expect(current!=151,0) && alwaysEnemySeen` (`x1`) | 6,313 | 1,859 | 9,908 |
| reversed `&&` operand (`alwaysEnemySeen && current!=151`) (`m_n3`) | 6,309 | 1,858 | 9,908 (byte-identical) |
| `expect` on the whole OR (`m_n7`) | 8,551 | 1,814 | **9,904** |
| `needStop` with expect on mask (`m_n2`) | 6,332 | 1,856 | 9,908 |

`ep_reorder_exp` is the only body that reproduces the original's inline else *and* the
original's evaluation order, and its music block (`130274`–`130330`) is close to the
original instruction-for-instruction — but the enemy loop re-colour costs ~950 raw, so it
is 978 worse overall. The outer expect is load-bearing for the enemy-loop layout.

## 3. The wreck x/y block is the first structural tail divergence

Original `12fc40`–`12fcac` (108 B) vs base `12fc40`–`12fca0` (96 B): the base is **12 B
shorter**. The logic is identical in both; the difference is codegen:

* the original **rematerialises** `ldr r2,[r4,#252]` (screenX) in four separate paths
  (`12fc6a`, `12fc70`, `12fc88`, `12fc94`);
* the base **CSE-hoists** it once before the `0x11e` test (`12fc5e ldr r2,[r4,#252]`) and
  adjusts it in registers.

Every source spelling tried either keeps the hoist or changes the size:

| body | raw | size |
|---|---:|---:|
| base | **6,309** | 9,908 |
| `xc` (nested `if (0x11e) { if (0x11f) ... else if (!0x120) ... }`) | 6,329 | 9,908 |
| `w1` (`else { x = screenX; if (0x11f) y=screenY+11; else if (0x11e && !0x120) x-=11; }`) | 7,010 | **9,912** |
| `w2` (merge the two `0x11f` arms) | 7,423 | **9,904** |
| `w3` (nested `0x11e`/`0x11f` mirroring the original test order) | 7,583 | **9,948** |
| `w4`/`w5` (`int x=screenX; int y=screenY;` then adjust) | 7,100 | **9,900** |
| `w6` (assign `y` in every arm) | 7,511 | **9,944** |
| `w7` (merge the two `0x11f` arms, keep final y adjust) | 6,309 | 9,908 (byte-identical) |
| `w8` (y-adjust block before x) | 7,305 | **9,932** |
| `w9` (`int y = screenY;` then `if (0x11f) y+=11; else if (0x120) y-=11;`) | 7,262 | **9,940** |

`w4`/`w5` are the most compact (82 B block) and are 8 B short. No spelling reaches the
original's 108 B block at size 9,908.

The `if (this->field_0x1c == nullptr && wreck && this->tractorMode2)` block immediately
after the wreck block is the *inverse*: the original evaluates `tractorMode2` first
(`12fcb8 ldrb r0,[r4,#431]; cbz`), the base evaluates `field_0x1c`/`wreck` first and
materialises a 0/1 value. Reordering the source to the original's order is size-exact but
slightly worse:

| order | raw | size |
|---|---:|---:|
| base `A && B && C` (`0x1c`, wreck, tractor) | **6,309** | 9,908 |
| `A && C && B` (`base_tr3`) | 6,310 | 9,908 |
| `C && A && B` (`tract`/`base_tr1`/`tract_CAB`) | 6,311 | 9,908 |
| `A && C && B` + `expect(alwaysEnemySeen,0)` (`pp_tr3_n4`) | 6,314 | 9,908 |
| `B && A && C` (`base_tr2`) | 7,561 | **9,904** |
| `C && B && A` (`tract_CBA`) | 7,554 | **9,940** |

## 4. The 22-byte early-region diff is entirely the campaign spill

`12e050`–`12e3a4` is byte-identical except:

* `12e07a mov.w fp,#0` vs `sl,#0`, `12e086`/`12e08c`/`12e0d8` `strb.w fp` vs `sl`
  (shared-zero register);
* `12e0f8 mov sl,r0` vs `str r0,[sp,#0x90]` (campaign in `sl` vs spilled), and the
  redundant `str r0,[sp,#0x90]; ldr r0,[sp,#0x90]; cmp r0,#2` at `12e38a` vs
  `cmp.w sl,#2`;
* two stack-slot immediates (`0x94` vs `0x98`, `0x70` vs `0x74`) and 6 branch-offset bytes.

This is exactly the campaign-in-`sl` skeleton. Restoring it costs more elsewhere: the
`6721` body (campaign-in-`sl`, `volatile autoPilotTarget`, `String dist`, direct-OR
`otherLock`, `int marked`, `bossSeen ||`) has an early-region diff of **184** (vs 22) and
a planet-loop diff of 907 (vs 953) — net +116. The early cost is the waypoint block, which
the base matches exactly and the campaign-in-`sl` bodies re-colour.

## 5. Full variant sweep (232 bodies) and pairwise cross product

Every `scratch/w13346/v/*.cpp` body (232 files, accumulated across the wave-189 passes)
was rebuilt and measured in this worktree (`batchall.sh`). **No size-exact body beats
6,309.** The size-exact near-misses:

```
6309  base (and 25 byte-identical spellings)
6310  base_tr3 / tract_ACB
6311  base_tr1 / tract / tract_CAB / gh_char
6313  m3 / m_n4 / x1 / x2 / x3
6324  ord1 / ord2 / ord3
6329  xc / base_xy_nested / t2 / base_xy2_n2 / ...
6332  m_reorder_exp / m_n1 / m_n2 / m_n5 / m_B0b
6421  xy_n120
6721  6721 family
7287  ep_reorder_exp
```

A pairwise cross product of the best near-misses (`{tr3, n4, x1, re, ord1, xc}`) was also
measured; the best combination is `pp_tr3_n4` / `pp_tr3_x1` at **6,314** (size 9,908).

## 6. Conclusion

The base's 6,309 is a strong local optimum. Three independent blockers are now pinned to
exact instructions:

1. the music-block else placement (the original's `blt.n 1302f0` inline else) is controlled
   by the outer `expect(field_0x1bc>=1,1)`; removing it re-colours the enemy loop and costs
   ~950 raw;
2. the wreck x/y block is 12 B shorter than the original because clang CSE-hoists the
   `screenX` load; every source spelling that changes this either keeps the hoist or
   changes the function size;
3. the 22-byte early diff is the campaign spill, and the campaign-in-`sl` skeleton that
   fixes it re-colours the waypoint block and costs more than it saves.

The concrete next lever (not landed): make the wreck block 12 B longer (rematerialised
`screenX`) *and* remove 12 B from the following asteroid-lock block, so the whole
`12fc4c`–`13008c` region realigns; that is the only path to a large raw win, and it
requires reconstructing both blocks together.

No size-exact, `.rel.plt`-identical raw win was found on the assigned cells; `src/` is
left at the base.

## 7. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6309 aligned= 1858 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13346/`: `Radar.base.cpp` (6,309 base),
`run.sh` / `measure.sh` (build+measure), `regdiff2.py` (correct per-region diff),
`heat2.py` (byte-diff heat), `idiff.py` (instruction-aligned diff), `batchall.sh` +
`batchall.log` (232-body sweep), `v/` (all variants, including `ep_reorder_exp`,
`m_n4`, `xc`, `w1`..`w9`, `tract_*`, `pp_*`), `orig.dis` / `base_now.dis` /
`epre.dis` / `mre_now.dis` / `xc.dis`.
