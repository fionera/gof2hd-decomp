# Radar::draw wave 191b (w13528) — sixth lens: gas-spark `+` form, gas 24000 axis/sign
# matrix, type matrix, distant statement swaps; no size-exact raw win

**Status:** analysis complete, 2026-11-12. **No size-exact raw win found**; `src/` is left at
the base. This pass commits only this note. Worktree `wt/w13528`, branch `wave191b-w13528`,
base `master` `ce4bd3c6c`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave191b-w13520.md` (fifth lens, current 6,016 base),
`radar-draw-wave191b-w13510.md` (fourth lens), `radar-draw-wave191b-w13507.md` (local
sweeps) and `radar-draw-wave189-w13319.md` (the `campaign`-in-`sl` register map).  Every
number below was measured in this worktree on the **current** 6,016 base.

## 0. Baseline (verified, unchanged)

| metric | value |
|---|---:|
| function size | 9,908 / 9,908 |
| raw same-address diff | **6,016** |
| instruction-index alignment | 1,941 |
| instruction count | 3,541 ours / 3,532 orig |
| frame | `sub sp,#296` (`0x128`) |
| `mode` argument slot | `sp+0x68` (original `sp+0x64`) |
| `campaign` | spilled `sp+0x90` (original `sl`) |
| shared zero / planet counter | `sl` (original `fp`) |
| planet-loop `stations` | spilled `sp+0x84` (original `sl`) |
| `.rel.plt` | byte-identical |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` with `src/` at the base. No code change is committed by this pass.

## 1. Gas-spark `inSight` — the `+`/`int top` form is *instruction-identical* but raw-worse

The original (`12f4a8`-`12f4f8`) computes **both** `left` and `top` as
`(int)(crosshairPos - size)` up front and uses the `+` upper-bound form
(`screenX < left + 2*size`, `screenY < top + 2*size`).  Rewriting `Radar.cpp:969` to

```cpp
int top = (int) (PlayerEgo::crosshairPos.y - (float) size);
inSight = this->screenX > left && this->screenX < left + size * 2 && this->screenY > top &&
          this->screenY < top + size * 2;
```

reproduces the original **instruction for instruction** (registers `s0/s2/s4`, `r0/r1/r2/r3`,
`vsub s4/s2`, `vcvt s0/s2`, `add.w r2,r2,r0,lsl#1`, `add r0,r1` all match; verified by
side-by-side disassembly — the only differences are PC-relative offsets from the 12 B
shift).  It is nevertheless a raw **loss** because the shift in the gas block is `+0x0c`
and the base's `-` form happens to align more bytes there:

| cell | raw | aligned | note |
|---|---:|---:|---|
| base (`-` form, `float top`) | **6,016** | 1,941 | |
| `+` form, `int top` (`SPint`) | 6,032 | 1,916 | sequence matches orig exactly |
| `+` form, `float top` (`SPflt`) | 6,020 | 1,937 | |
| `SPint` + enemy-24000 upper-first | 6,032 | 1,929 | |
| `SPflt` + enemy-24000 upper-first | 6,020 | 1,950 | |
| `SPint` + gas-24000 x-y-z ego-first | 6,037 | 1,916 | |
| `SPflt` + gas-24000 x-y-z ego-first | 6,025 | 1,937 | |

Per-`0x100`-bucket equal-byte accounting: the only bucket that moves between base and
`SPint` is `0x12f400` (base 76 equal bytes, `SPint` 60) — the `+` form is a pure −16 in
that one bucket.  The correct source shape is recorded here for a future pass whose shift
is smaller.

## 2. Gas-cloud 24000 chain — original is x-y-z, ego−target, upper-first

The original (`12f64c`-`12f6aa`) tests **x, then y, then z**, subtracting
`egoPos - targetPos` (x via the `r5`/`fp` vector pointers, y/z via `r4+#0x170/#0x17c` and
`r4+#0x174/#0x180`), upper bound (`vcmpe s0,s16; bgt`) before lower bound
(`vcmpe s0,s18; bmi`).  The source (`Radar.cpp:1004`) is x-z-y, `targetPos - egoPos`,
upper-first.  All 6 axis orders × 2 subtraction signs were measured:

| cell | raw | size | note |
|---|---:|---:|---|
| base (x-z-y, target−ego) | **6,016** | 9,908 | |
| x-y-z, target−ego | 6,018 | 9,908 | instruction order matches orig |
| x-y-z, ego−target | 6,021 | 9,908 | matches orig exactly in form |
| x-z-y, ego−target | 6,019 | 9,908 | |
| y-x-z / y-z-x (either sign) | 8,176-8,196 | **9,912** | fatal |
| z-x-y / z-y-x (either sign) | 8,150-8,168 | **9,904** | fatal |

The original's *source* is therefore x-y-z ego−target upper-first, but the 12 B shift in
the gas block means matching it costs raw bytes (the `vcmpe` immediates and PC-relative
offsets still differ).  Base is optimal.

## 3. Tractor 24000 chain — verified byte-identical

`Radar.cpp:893` (`egoPos - targetPos`, lower-first, x-y-z) is **byte-identical** at
`1300e4`-`130142` (same addresses, same `vcmpe s0,s18; bmi` then `vcmpe s0,s16; bgt`
per axis, including the `vldr s0,[r6]` / `vldr s2,[r5]` pointer form for x and the
`[r4,#0x170]`/`[r4,#0x17c]` offset form for y/z).  Do not touch it.  The station-blip
block is likewise already byte-identical (`12f8af` len 111, `12f91f` len 383).

## 4. Planet box — base is optimal

All six term orderings / bound forms of `Radar.cpp:600` were re-measured on the current
base: base (y-pair first, `top + size > screenY` / `screenX < left + size` upper-first)
= **6,016**; x-pair-first lower-first = 6,022; x-pair-first upper-first = 6,019;
y-pair-first lower-first = 6,022; `size/2` instead of `size >> 1` = 8,701 / **9,924**
(fatal).  `(int) i` vs `i` in the size ternary is code-identical (6,016).

## 5. Exhaustive local-type matrix (all 4-byte locals × 10 types)

Every `bool`/`int` local in `draw` was retyped to each of
`bool,int,unsigned,char,short,uint32_t,int32_t,uint8_t,int8_t,long` one at a time.
Only the following were size-exact **and** raw ≤ 6,016; nothing beat the base:

| local | best size-exact raw | note |
|---|---:|---|
| `alienOrbit` int→char/short/long/uint*/int* | 6,016 | code-identical |
| `distance` int→unsigned/char/uint32/int32/long | 6,016 | |
| `meters` / `remainder` int→char/short/int32/uint8/int8 | 6,016 | |
| `distance` int→bool | 6,018 | |
| `meters`/`remainder` int→bool | 6,017 | |

All `bool` locals retyped to `int`/`unsigned`/etc. are size-fatal (9,912-9,952) except
`alienOrbit` (6,025).  This re-confirms w13520 §1.5/§1.6 on the current base.

## 6. Distant statement swaps (40-400 lines apart)

The prior waves exhausted same-brace-depth simple-statement swaps at distance ≤ 40 lines.
90 random same-depth swaps at distance 40-400 lines were measured: only 2 were size-exact
(`508<->677` = 6,016, `552<->871` = 6,519); the rest were size-fatal.  No win.

## 7. Aligned-improving neutral cells and their combinations

Several source shapes are **raw-neutral (6,016) but instruction-aligned better**; all were
combined pairwise/triple to look for a shift interaction, with no raw movement:

| cell | raw | aligned |
|---|---:|---:|
| base | 6,016 | 1,941 |
| enemy-24000 upper-first (`EN`) | 6,016 | 1,954 |
| tractor guard `A C B D` (`TR`) | 6,016 | 2,016 |
| `carriesFriendCargoFlag` expect 0→1 (`CF`) | 6,016 | 1,993 |
| `EN`+`TR` | 6,016 | **2,029** |
| `EN`+`CF` | 6,016 | 2,006 |
| `EN`+`TR`+`CF` | 6,016 | 1,960 |
| `offset` hoisted above `if (i != 3)` (`HO`) | 6,054 | 1,987 |
| any `HO` combination | 6,054 | 1,995-2,018 |

`EN`/`TR`/`CF` are genuine source-order corrections (the original really is upper-first /
`A C B D` / expect-1) but they are raw-neutral, so they do not satisfy the "net-positive
raw" landing rule.  `EN`+`TR` (aligned 2,029) is the closest match found for this function.

## 8. Root cause (confirmed, unchanged)

The residual is the `campaign`-in-`sl` / zero-in-`fp` allocator tie-break plus the
`mode`↔`offset` spill-slot swap (`sp+0x68` vs `sp+0x64`, and vice versa for the
landmark-loop `offset`).  The base's spilled-`campaign` layout happens to align more bytes
than any `campaign`-in-`sl` body: `bool otherLock` is 9,247/9,860, `bsor`
(`bool otherLock` + `int marked` + `bossSeen ||`) is **6,787**/9,908.  No declaration
order, type, spelling, operand order, `__builtin_expect` flip, De Morgan rewrite, or
statement move tried across waves 162-191 (and this pass) changes the tie-break without a
size change or a worse early spill.

## 9. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so --json _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6016 aligned= 1941 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13528/`: `sweep.py` (1 s
compile+link+fn_diff harness), `Radar_base.cpp` (6,016 base), `m1.py`-`m20.py` (the
cell sweeps above), `sbsr.py` (range-filtered side-by-side disassembler), `shift.py` /
`shiftv.py` (per-instruction shift profile), `base.so` / `spint.so` / `bsor.so`
(candidate builds for bucket comparison).
