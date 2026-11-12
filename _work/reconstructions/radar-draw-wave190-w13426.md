# Radar::draw wave 190 (w13426) — planet-lock store order (`field_0x18` before
# `planetDockIndex`); raw 6,289 -> 6,288 (size-exact, `.rel.plt` identical)

**Status:** landed, 2026-11-12. Worktree `wt/w13426`, branch `wave190-w13426`, base
`master` `32b826e31`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave190-w13412.md` (planet-lock y test, landed), `-w13404.md`
(spark `int top`), `-w13398.md` (24000-box order) and `radar-draw-wave189-w13346.md`
(wreck x/y / music-tail analysis).

## 0. The change

The planet-lock latch in the planet loop wrote `planetDockIndex` before caching the
locked planet pointer. The original (`12e6xx`) stores the pointer first and the index
second; swapping the two adjacent stores makes clang emit the original's store order
and removes one same-address byte:

```cpp
-                        this->planetDockIndex = i;
-                        this->field_0x18 = this->planetTargets->data()[i];
+                        this->field_0x18 = this->planetTargets->data()[i];
+                        this->planetDockIndex = i;
```

Both stores are to distinct members and the intervening expression
(`this->planetTargets->data()[i]`) is a pure read, so the swap is semantics-preserving.

| metric | base | after |
|---|---:|---:|
| raw same-address diff | **6,289** | **6,288** |
| instruction-index alignment | 1,912 | 1,913 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,543 / 3,532 | 3,543 / 3,532 |
| `.text` differing bytes (whole `.so`) | 72,922 | **72,921** |
| `.ARM.extab` differing bytes | 603 | 603 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| `.ARM.exidx` | byte-identical | byte-identical |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370. The function is already
non-byte-exact, so `byte_exact` is unchanged; the gain is one raw byte.

## 1. How it was found

A whole-function adjacent-statement-swap sweep (121 candidate pairs of same-indent
single-line statements that both end in `;` and are not control flow) was rebuilt one
pair at a time. This pair was the only raw improvement; the rest were byte-identical
(`raw=6289`) or worse. The same sweep re-confirmed the base's other adjacent pairs are
neutral.

## 2. Negative results measured this pass (all reverted)

Full incremental `link_so -j2` + `tools/fn_diff.py` (`ulimit -v 12000000`; scratch
`/home/debian/GalaxyOnFire2/scratch/w13426/`).

### 2.1 Station/planet frame math (the "divide-then-multiply" lever)

The original station (`13040c`) and planet (`1304a4`) blip frame blocks compute
`(timer / field_0x1b8) * (frames - 1)` (`vdiv` then `vmul`); the base computes
`(timer * (frames - 1)) / field_0x1b8` (`vmul` then `vdiv`). Every source spelling that
makes clang divide first was measured:

| spelling | raw | aligned | size |
|---|---:|---:|---:|
| base | **6,289** | 1,912 | 9,908 |
| station `int n = getRawFrameCount()-1; ratio = timer/field; frame = ratio*n` | 6,291 | 1,900 | 9,908 |
| planet, same shape | 6,291 | 1,943 | 9,908 |
| both | 6,293 | 1,892 | 9,908 |
| station `float ratio = timer/field; frame = ratio*(frames-1)` | 6,290 | 1,913 | 9,908 |
| planet, same | 6,294 | 1,914 | 9,908 |
| both | 6,295 | 1,915 | 9,908 |
| station `float frames` then `timer/field*frames` | 6,291 | 1,900 | 9,908 |
| both (`float frames`) | 6,293 | 1,892 | 9,908 |

The divide-first forms reproduce the original's 14-instruction station block
**instruction-for-instruction** (`mov r0,r6; blx getRawFrameCount; vldr s0,[r4,#420];
subs r0,#1; vldr s2,[r4,#440]; 2x vcvt; vdiv; vmov s2,r0; vcvt; vmul; vcvt.s32; vmov r1;
b.n`), but the block sits 0x70 B earlier than the original (the enemy/asteroid cold-block
placement), so same-address raw cannot improve and the extra `vmov`/`vcvt` pair makes it
+2. The aligned metric improves (1,912 -> 1,900) but `.text` grows by 2, so it was not
landed.

### 2.2 `camZ` hoist (`const auto camZ = this->cameraZ;`)

The original loads `cameraZ` into a caller-saved register at the `calcDistance` call
(`12f6fa vstr s6,[sp,#12]`) and uses s16/s18 for the ±24000 box constants. The base
hoists `camZ` into s16 (`12f43c vldr s16,[r4,#580]`) and reloads two constants into
s18/s20 from a fresh pool at `12f48c`. Removing the hoist (`this->cameraZ` at the call
site) reproduces the original's gas-block prologue (`vldr s16,[pc]; vldr s18,[pc];
vstr s6,[sp,#12]`) and improves aligned 1,912 -> 1,890, but raw worsens to 6,364
(`.text` +75) because the 4-byte-shorter block changes the downstream shift. Not landed.

### 2.3 Other cells

* `otherLock` (`line 953`) operand orders (lockedStation first/last, `field_0x14`/`field_0x1c`
  first): 6,300 / 8,315 / 8,459 (size-fatal for the latter two).
* `!(isDockingToDockingPoint() || turretMode || otherLock)` reorders: 6,351 / 8,578 / 8,809.
* asteroid/gas `screenX < left + size*2` -> `left > screenX - size*2` subtraction form
  (lines 848, 1017): 6,294 each; x-only: 6,291 each.
* adding `__builtin_expect` to 246 plain `if` conditions (each value 0/1): no row beat
  6,289.
* `volatile bool landmarkHit` / `volatile bool planetHit`: 9,566 / 9,141 (size-fatal).
* `planetHit` hoisted to function scope / declared before `landmarkHit`: byte-identical.
* `12faf8` literal word (`0xe0ad6` vs `0xe0ad4`) is a PC-relative GOT offset for
  `Globals::*`; both resolve to the same target (`0x1d027c`) — the 2-byte difference is
  the shift of the preceding `add r2,pc`, not a content difference.
* The end pool is at `0x1306f0` in both (the handoff's `0x1306ec` was an objdump
  misread); the `0.325f/0.65f/100.0f/50.0f` words at `0xd2854` belong to
  `Radar::calcDistance`, not `draw`.

### 2.4 Exhaustive local-order sweeps (all reverted; base = 6,288 after the landed edit)

* adjacent same-indent `;`-statement swaps (121 pairs): only the landed pair wins;
  every other pair is 6,288.
* distance-2 statement swaps (44 triples): all 6,288.
* top-level `&&`/`||` adjacent-operand swaps (8 cells): 6,292 .. 9,079.
* `== 0` -> `< 1`, `!= 0` -> `> 0`, `== nullptr` -> `!x` (41 cells): all 6,288.
* `if (c) {A} else {B}` -> `if (!c) {B} else {A}` (22 cells): all 6,288.
* adjacent `if`-block swaps (20 cells): 6,318 .. 8,559.
* `bool` -> `uint8_t`/`char`/`signed char` for `landmarkHit`/`planetHit`: 8,482 ..
  8,706 (size-fatal).
* adding `__builtin_expect` to 246 plain `if` conditions: no row beat 6,288.

The residual raw is dominated by block placement: the `.text` shift profile oscillates
by -46 .. -158 B through the planet loop, enemy loop and music tail while returning to 0
at the end, so same-address bytes rarely line up even where the instruction streams match.

## 3. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6288 aligned= 1913 size=9908/9908 insn=3543/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13426/`: `autoswap.py` (121-pair sweep),
`ifexpect.py` (246-cell expect sweep), `variants19..29.py` (frame math, camZ, otherLock,
comparison, stack-slot cells), `cmpsweep.py`, `shiftmap.py`, `contentdiff.py`,
`branchdelta.py`, `secdiff.py`, `rawbytes.py`, `bytediff.py`.
