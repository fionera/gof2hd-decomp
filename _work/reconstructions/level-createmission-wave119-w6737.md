# w6737: `Level::createMission` (92.293729 -> 92.293729 %, 17172 B) — wave-119:
# case-183 default-body placement / turret-selector / stack-slot sweep; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6737`, branch `wave119-w6737`,
based on current master (`6552d3af5`). Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`-`0xb42ec`, 17172 B).

## Baseline (pristine, wave-118 `volatile unsigned n` latch)

```text
_ZN5Level13createMissionEv   official 92.293729 %   size 17172/17172 B
                             6065 ours / 6055 orig insns
gate: byte 4505/4524, linked 4505/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
ELF: 27 sections, file 2,187,328 B, .ARM.extab 0x26290 (8 B short, createShip only).
```

## 1. Result

No source variant beat the baseline.  The best measured score remains
**92.293729 % / 17172 B / 6065 insns**.  `src/game/world/Level.cpp` is unchanged
(this note is the only commit).

## 2. New finding: first address divergence is the case-183 default body

`tools/verify/verify.py` normalises branch targets to `loc_*`, so the raw
instruction-address stream was compared directly.  The first place where the
two builds' instruction addresses differ is **instruction 1901**:

```text
orig 0x144e  str r1, [sp, #380]      ; shared final store of the case bodies
ours 0x1450  str r1, [sp, #372]
```

Everything before `0x144e` has identical addresses (the earlier `cmp`/`beq`
test-chain difference is compensated inside the switch block).  The cause is
the **default body placement**:

```text
orig: body0, body1, body2, default(inline), shared str [sp,#380]
ours: body2, body0, body1, shared str [sp,#380], b join, default(out-of-line)
```

The original's default body ends with `offset[2]` and falls through to the
shared `str [sp,#380]` (10 bytes).  Ours stores `offset[2]` itself and is
out-of-line after the join (12 bytes), adding the `b.n` over it.  Source order
`offset[1]; offset[0]; offset[2];` is the order that would share the store,
but in this build it makes LLVM materialise the constants in VFP registers
(`vmov.f32 s0, s20`, ...) and the function grows to 17188 B:

| default-body source order | official % | size | insns |
|---|---|---|---|
| `1,2,0` (baseline) | **92.293729** | 17172 | 6065 |
| `2,0,1` | 92.277228 | 17172 | 6065 |
| `2,1,0` | 92.277228 | 17172 | 6065 |
| `0,2,1` | 91.774606 | 17172 | 6066 |
| `1,0,2` (shares the store, but vectorised) | 89.653465 | 17188 | 6065 |
| `0,1,2` | 88.606760 | 17224 | 6075 |

`(float)-45000`, `-45000`, a named `float z`, and `-45000.0f + 0.0f` all
vectorise identically.  A `Vector offset` local adds the default-ctor zero
stores (17192 B / 89.538).

## 3. Case-183 body order is not reachable by source case order

The original block order is `0,1,2,default` with tests `2,1,0`.  The build's
machine block order is a function of the first IR case only:

| source order | machine body order | official % |
|---|---|---|
| `0,1,2` | `0,2,1,D` | 92.145215 |
| `0,2,1` | `0,2,1,D` | 92.145215 |
| `1,0,2` | `1,0,2,D` | 92.161716 |
| `1,2,0` | `1,0,2,D` | 92.161716 |
| `2,0,1` | `2,0,1,D` | 92.293729 |
| `2,1,0` | `2,0,1,D` | 92.293729 |

`switch (k)` with every permutation of cases 0/1/2 and default first/last,
nested if/else, `if (k<3)/else`, `if (k>2)/else switch`, and separate-if +
`goto` were all measured; none produces `0,1,2,default`.  `if (k>2) {...}
else switch (k)` does produce `0,1,2` but adds the `k>2` test (6066 insns,
91.890108).

## 4. Turret selector: branch vs if-conversion

Original (`b32c6`): `ldr r0,[sp,#64]; cbz; ldr;add;b;ldr;add` — a real branch.
Baseline anchor (`(bigAlienWave + (i < 0))`): `ldr;add` both arms then
`cmp.w r1,r9,asr #31; it ne; movne` — if-converted.  The anchor is still the
only form that keeps the select in-loop *and* preserves 17172 B.

Variants measured this wave (all 17172 B unless noted):

| variant | official % | insns |
|---|---|---|
| baseline anchor | **92.293729** | 6065 |
| `(bigAlienWave + (i <= -1))` / `(i >> 31)` / `(int)(i<0)` | 92.293729 | 6065 |
| `(bigAlienWave + (i<0) + (k<0))` / `(i<0 \|\| k<0)` | 92.293729 | 6065 |
| `(bigAlienWave != 0) != (i<0)` / `^` | 92.293729 | 6065 |
| `(bigAlienWave + (k<1))` | 92.112211 | 6065 |
| `(bigAlienWave \| (i<0))` | 92.244224 | 6065 |
| `(bigAlienWave + (i<1))` | 90.901592 | 17180 |
| `bool stationTable; if (i<0) toggle` | 90.475798 | 17188 |
| `(bigAlienWave + (k<0))` / `(k>>31)` / `(i<0 && k<0)` | 88.380543 | 17140 |
| plain ternary (no anchor) | 88.380543 | 17140 |
| plain ternary + `volatile int bigAlienWave` | 87.700000 | 17140 |
| `volatile int` + anchor | 87.700000 | 17140 |
| `bool bigAlienWave` plain / if-else | 87.078652 | 17124 |
| `bool bigAlienWave` + anchor | 89.228229 | 17156 |
| `volatile bool` / `volatile uchar` plain | 86.344122 / 87.938578 | 17148 |
| `volatile` pointer `t` | 68.902189 | 17244 |

The faithful body (cached `gun`, `setScaling` after `setPosition`, cargo before
shipGroup) is 92.160423 / 17172 / 6063; with the fresh `(*enemies)[i]`
setScaling receiver 92.227723 / 17172 / 6065.  The baseline's
`setScaling`-before-`setRotation` + shipGroup-before-cargo remains best
(92.293729).  A full sweep of statement orders A(select) B(rotate) C(scaling)
D(setRotation) E(rotate2) F(setPosition) G(cargo) H(shipGroup) found nothing
above baseline; `ABDEFCGH` (faithful) = 92.160423, `ABDEFGHC` = 92.211221,
`ABDCEGHF` = 92.260726, `ABCDEFG H` with cargo before shipGroup = 92.277228.

## 5. Stack-slot swap is a Stack-Slot-Coloring renumbering, not source order

MIR after Virtual Register Rewriter: `rnd = %stack.74` (11 uses),
`raid = %stack.86` (20 uses).  After Stack Slot Coloring: `rnd = %stack.84`,
`raid = %stack.83`.  The final frame layout maps higher `fi#` to lower
`sp` offset, so `stack.83` lands at `sp+88` and `stack.84` at `sp+84` (ours).
The original has the opposite (`raid` at `sp+84`, `rnd` at `sp+88`), i.e. the
coloring renumbered the two 4-byte slots in the opposite order.  Source
declaration/initialisation/type changes (`bool/int/unsigned/char/short`,
`= false`, `= cond ? ...`) do not move it; the slot order is decided inside
the coloring pass from the spill use counts (raid 17 disasm refs vs rnd 2).

A local `AERandom *rnd = Globals::rnd;` used throughout the function changes
the whole allocation (61.137323 %, 17116 B / 6026 insns) — not the original
shape.

## 6. case4Tail `size()` latch

`volatile unsigned n` + `i < n` is the baseline (92.293729, 6065).  The
original reloads `this->enemies->size()` each iteration.  Measured:

| variant | official % | size | insns |
|---|---|---|---|
| baseline `volatile n`, `i < n` | **92.293729** | 17172 | 6065 |
| no latch, `i < size()` | 91.308296 | 17164 | 6060 |
| `volatile n` unused, `i < size()` | 90.867090 | 17176 | 6066 |
| `volatile n; (void)n; i < size()` | 90.951085 | 17180 | 6068 |
| `i < n && i < size()` | 91.116060 | 17184 | 6068 |
| `i < (n \| size())` | 90.835602 | 17180 | 6068 |

## 7. Residual analysis (unchanged)

* `this` r4 (orig) vs r5 (ours) from the alien-turret preheader onward;
  `gun` r6 vs r4; `&return-slot` r5 vs r8; `k*24` r9 vs fp; `i` fp vs r9;
  `t` sl vs r6.  Dominant residual (~2.7 points).
* Stack-slot swap `raid` (orig sp+84 / ours sp+88) vs `Globals::rnd`
  (orig sp+88 / ours sp+84); 17 refs each way.
* Turret selector branch (orig) vs if-converted select (ours).
* Case-183 default body inline+shared store (orig) vs out-of-line (ours);
  first address divergence at `0x144e`.
* `&&` at 1904/1907: orig ORs both negated operands, ours short-circuits.
* `cmp #0;ble` (orig) vs `cmp #1;blt` (ours) at 1837.
* `.ARM.extab` still 8 B short (`0x26290` vs `0x26298`, createShip only);
  27 sections and `2,187,328 B` file size unchanged by every variant above.

## 8. Commands

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6737/score.py            # official % / size / insns
python3 /dev/shm/w6737/diffmap2.py         # per-cluster census + source lines
cmake --build cmake-build-match --target check -j6
```

Scratch tools (not tracked): `/dev/shm/w6737/{score,diffmap2,sweep,sweep183,
sweep183b..f,sweeploop,sweepbaw}.py`, `Level.base.cpp` (== HEAD), `Level.s`,
`Level.ll`, `mir_all.txt`, `diff2.txt`.
